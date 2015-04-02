#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;


#define CHAR unsigned char
#define FAULT 255
const CHAR TAILLE		= 32;
const CHAR TAILLEPAGE	= 4; 


class CTableEntry
{
private:
	CHAR Segment;
	int NoSwap;
	CHAR Cadre;
	CHAR RWX;
	char Process;
	unsigned int RNFU;
	bool MBitsNFU;
	bool RBitNFU;


public:
	CTableEntry(CHAR segment, int noSwap, CHAR cadre, CHAR rwx, char process)
	{
		Segment =	segment;
		NoSwap =	noSwap;
		Cadre =		cadre;
		RWX =		rwx;
		Process =	process;
		RNFU = 0;
		MBitsNFU = false;
		RBitNFU = false;
	}

	CHAR getCadre()		{	return Cadre;		}
	CHAR getSegment()	{	return Segment;		}
	int getNoSwap()		{	return NoSwap;		}
	char getProcess()	{	return Process;		}
	unsigned int getR() {   return RNFU;		}
	bool getBitR()		{   return RBitNFU;		}
	bool getM()			{   return MBitsNFU;	}		
	CHAR getProtection(){   return RWX;			}
	
	
	void setBitR(bool r){	RBitNFU = r;		}

	void setR(unsigned int newR)
	{
		RNFU = newR;
	}
	void setM(bool newM){
		MBitsNFU = newM;
	}

	void setCadre(CHAR cadre)	{	Cadre = cadre;	}
};


class CProcesseur
{
	CHAR*			RAM;
	CTableEntry**	TableDesCadres;

	CHAR			Registre;
	CHAR			PC;				// Program Counter
	CHAR			State;
	CTableEntry***	PageTable;

public :
	CProcesseur()
	{
		RAM = new CHAR[TAILLE*2];
		for (int x=0; x < TAILLE*2; x++) RAM[x] = FAULT;

		TableDesCadres = new CTableEntry*[TAILLE*
			2/TAILLEPAGE];
		for (int x=0; x < TAILLE*2/TAILLEPAGE; x++) 
			TableDesCadres[x] = NULL;

		Registre	= 0;
		PC			= 0;
		State		= 'E';
	}

	~CProcesseur()						{ delete RAM;			}	

	CHAR	retPC()						{ return PC;			}
	CHAR	retRegistre()				{ return Registre;		}
	CHAR	retState()					{ return State;			}
	void setPC(CHAR pc)					{ PC = pc;				}
	void setRegistre(CHAR registre)		{ Registre = registre;	}
	void setState(CHAR state)			{ State = state;		}
	void setTable(CTableEntry*** table)	{ PageTable = table;	}
	void setRAM(CHAR indice, CHAR val)	{ RAM[indice] = val;	}

	CHAR resolve(CHAR data, CHAR segment)	
	{
		CHAR page = (CHAR)((data & 252)/4);		// ou data >> 2
		CHAR deplacement = (data & 3);

		CHAR cadre = PageTable[segment][page]->getCadre();

		return (cadre != FAULT ? 
				PageTable[segment][page]->getCadre()*TAILLEPAGE + deplacement : 
				FAULT);
	}
	

//*************************
// MOD START 
CTableEntry* getPageFault()	
	{
		if(PageTable[0][(PC & 252)/4]->getCadre() == FAULT)
			return PageTable[0][(PC & 252)/4];
		
		CHAR instruction = resolve(PC, 0);
		CHAR no	= (RAM[instruction] & 224) >> 5;
		CHAR data (RAM[instruction] & 31);
		
		if (no >=1 && no <= 4) 
		{
			CHAR page = (CHAR)((data & 252)/4);
			if (PageTable[1][page]->getCadre() == FAULT)
				return PageTable[1][page];
		}
		if (no == 7)
		{
			if (data == 0)
			{
				CHAR page = (CHAR)((Registre & 31 & 252)/4);
				if (PageTable[1][page]->getCadre() == FAULT)
					return PageTable[1][page];
			}
		}

		return NULL;					// pas de défaut de page à cette instruction
	}
	
	void MiseAJour(CHAR no, CHAR data, CHAR adresseProg, CHAR adresseData)
	{
		////////////////////////////////////////////////////////////////
		// Initialisation des variables
		////////////////////////////////////////////////////////////////

		CHAR verifProtection;

		////////////////////////////////////////////////////////////////
		// Référencement de l'instruction
		////////////////////////////////////////////////////////////////

		if(adresseProg!=255)	//n'est pas en pagefault
		{
			//cout << "Adresse prog: " << (int)adresseProg << endl;
			CHAR cadreInstruction = PageTable[0][(adresseProg/TAILLEPAGE)]->getCadre();
			// Référence le cadre de l'instruction
			if(cadreInstruction != 255)
			{
				verifProtection = TableDesCadres[cadreInstruction]->getProtection() & 5;
				if(verifProtection != 5)	//Protection
				{
					cout << endl << "Exception: Il est impossible d'effectuer cette action sur cette case mémoire. Protection: " << TableDesCadres[cadreInstruction]->getProtection() << endl;
					std::exit(0);
				}
				TableDesCadres[cadreInstruction]->setBitR(true);
		
				unsigned int RnumberInstruction = TableDesCadres[cadreInstruction]->getR();
				RnumberInstruction /= 2;
				RnumberInstruction += 2147483648;
				TableDesCadres[cadreInstruction]->setR(RnumberInstruction);
			}
		}

		//////////////////////////////////////////////////////////////////
		// Référencement des données
		//////////////////////////////////////////////////////////////////
		if(adresseData != 255)
		{//cout << "adresseData: " << (int)adresseData << endl;
			int index = (adresseData/TAILLEPAGE);
			
			CHAR cadreDonnee = PageTable[1][((adresseData-32) / TAILLEPAGE)]->getCadre();	// on veut le cadre libre
			
			if(cadreDonnee != 255)
			{
				// Référence le cadre des données
				if(TableDesCadres[cadreDonnee]->getBitR() == false)	//Être sure qu'on n'a pas de conflit avec l'instruction
				{
					if(no == 2)
					{
						verifProtection = TableDesCadres[cadreDonnee]->getProtection() & 3;
						if(verifProtection != 3)	//Protection
						{
							cout << endl << "Exception: Il est impossible d'effectuer cette action sur cette case mémoire (Écriture). Protection: " << TableDesCadres[cadreDonnee]->getProtection() << endl;
							std::exit(0);
						}
						TableDesCadres[cadreDonnee]->setBitR(true);
						TableDesCadres[cadreDonnee]->setM(true);	//Reviens à mettre le bit M à 1
						unsigned int Rnumber = TableDesCadres[cadreDonnee]->getR();
						Rnumber /= 2;
						Rnumber += 2147483648;
						TableDesCadres[cadreDonnee]->setR(Rnumber);
					}

					else if (no == 1 || no == 3 || no == 4)
					{
						verifProtection = TableDesCadres[cadreDonnee]->getProtection() & 1;
						if(verifProtection != 1)	//Protection
						{
							cout << endl << "Exception: Il est impossible d'effectuer cette action sur cette case mémoire.(lecture) Protection:" << TableDesCadres[cadreDonnee]->getProtection() << endl;
							std::exit(0);
						}

						TableDesCadres[cadreDonnee]->setBitR(true);
						unsigned int Rnumber = TableDesCadres[cadreDonnee]->getR();
						Rnumber /= 2;
						Rnumber += 2147483648;
						TableDesCadres[cadreDonnee]->setR(Rnumber);
						//cout << "Save Rnumber test: " << Rnumber << endl;

					}
					else if (no == 7)//adresse Data different, : adresseData = resolve(Registre & 31, 1);
					{
						if(data == 0)
						{
							verifProtection = TableDesCadres[cadreDonnee]->getProtection() & 1;
							if(verifProtection != 1)	//Protection
							{
								cout << endl << "Exception: Il est impossible d'effectuer cette action sur cette case mémoire.(lecture) Protection:" << TableDesCadres[cadreDonnee]->getProtection() << endl;
								std::exit(0);
							}
							adresseData = resolve(Registre & 31, 1);

							cadreDonnee = PageTable[1][(adresseData-32)/TAILLEPAGE]->getCadre();
			

							TableDesCadres[cadreDonnee]->setBitR(true);
							unsigned int Rnumber = TableDesCadres[cadreDonnee]->getR();
							Rnumber /= 2;
							Rnumber += 2147483648;
							TableDesCadres[cadreDonnee]->setR(Rnumber);
							//cout << "print Rnumber test: " << Rnumber << endl;
						}
						// Problème : trouver le bon cadre de page, même si adresseData à changer.
						// Est-ce que même la même page?
					}
				}
			}

		}

	if(adresseData != 255 || adresseProg != 255)
	{
		for(int x = 0; x < 2; x++)
		{
			for(int y = 0; y < TAILLE/TAILLEPAGE;y++)
			{
				if(PageTable[x][y]->getCadre() != 255)
				{
					if(TableDesCadres[PageTable[x][y]->getCadre()]->getBitR() == false)
					{
						unsigned int Rnumber = TableDesCadres[PageTable[x][y]->getCadre()]->getR();
						Rnumber /= 2;
						TableDesCadres[PageTable[x][y]->getCadre()]->setR(Rnumber);
						//cout << "-----------NFU Decalage non Reference---------------:" <<  TableDesCadres[PageTable[x][y]->getCadre()]->getR() << endl;

					}
					else
					{
						TableDesCadres[PageTable[x][y]->getCadre()]->setBitR(false);
						//cout << "----NFU Decalage Reference------:" <<  TableDesCadres[PageTable[x][y]->getCadre()]->getR() << "x,y" << x << "," << y << endl;
					}
				}
			}
		}
	}

	}

	void loadPage(CTableEntry* page)
	{
		CHAR cadreLibre = getCadreLibre(page->getSegment());
		
	
		page->setCadre(cadreLibre);
		TableDesCadres[cadreLibre] = page;
		//cout << "cadre:" << (int)cadreLibre << endl; ///////////f1//////////////////

		ifstream file("Swap.cpp", ios::binary);		
		file.seekg(page->getNoSwap());

		for (int y=0; y < TAILLEPAGE; y++)
			file >> std::noskipws >> RAM[cadreLibre*TAILLEPAGE+y];
		file.close();
	}
	
	CHAR getCadreLibre(CHAR segment)	// TO DO: votre code implante un algorithme de changement de page NFU
	{
		unsigned int minim = 4294967295;
		CHAR cadre = 0;
		bool nonmodVerifiable[8];
		bool tousModifie = true;
		// Vérifie initialement si les cadres ont été modifié. Si c'est les cas, on évite de les changer à tout prix.

		for(int y = 0; y < TAILLE/TAILLEPAGE;y++)	// Vérifie initialement si les cadres ont été modifié. Si c'est les cas, on évite de les changer à tout prix.
		{
			if(TableDesCadres[segment * 8 + y] != NULL)
			{
				if(TableDesCadres[segment * 8 + y]->getM() == true)
				{
					nonmodVerifiable[y] = false;
				}
				else
				{
					nonmodVerifiable[y] = true;
					tousModifie = false;
				}
			}
			else
			{
				// le cadre est vide, donc non modifié
			}
			{
				nonmodVerifiable[y] = true;
				tousModifie = false;
			}
		}
		if (tousModifie == true)
		{
			fill(nonmodVerifiable,nonmodVerifiable + sizeof(nonmodVerifiable),true); //Rempli le tablea à true, car ils faut tous les considérés s'ils ont tous été modifiés
		}

		// Les cadres sont tous non modifié, on doit donc tous les considérer.

		//Vérifie s'il y a des cadres de pages vides. Si on , on le sélectionne immédiatement
		for(int y = 0; y < TAILLE/TAILLEPAGE;y++)
		{
			if(TableDesCadres[segment * 8 + y] == NULL)
			{
				cadre = segment * 8 + y;
				//noswap has no NoProcess
				//noprocess is always 0
				
				TableDesCadres[cadre] = new CTableEntry(segment,
					PageTable[segment][y]->getNoSwap(),
					cadre,
					1 |						// read			(1 = read only, 3= rw,5 = rx)
					(segment==1 ? 2 : 0) |			// write
					(segment!=1 ? 4 : 0),
					PageTable[segment][y]->getProcess());

				break;	// On a choisit le cadre, on peut arrêter de chercher.
			}


			//On choisit alors un nouveau cadre, ayant été référencé il y a le plus longtemps posible.
			if((TableDesCadres[segment * 8 + y]->getR() < minim) && (nonmodVerifiable[y] == true))	// &&: Si le cadre à été modifié et d'autres non, on ne le choisit pas automatiquement
				{
					minim = TableDesCadres[segment * 8 + y]->getR();
					cadre = segment * 8 + y;
				}
				//cout << "-----------NFU Decalage non Reference---------------:" <<  TableDesCadres[PageTable[x][y]->getCadre()]->getR() << endl;
		}
		if (TableDesCadres[cadre]->getM() == true)
		{
			TableDesCadres[cadre]->setM(false);
			save(cadre);
		}	
		
		TableDesCadres[cadre]->setCadre(FAULT);
		TableDesCadres[cadre] = NULL;
		return cadre;	
	}

	void save(CHAR cadre) 
	{
		ofstream file("Swap.cpp", ios::binary | ios::out | ios::in | ios::ate);
		if (file.is_open() == 0)
		{
			cout << "Erreur ecriture" << endl;
			exit(-1);
		}
		
		file.seekp(TableDesCadres[cadre]->getNoSwap());
		
		for (int y = 0; y < TAILLEPAGE; y++)
			file << RAM[cadre*TAILLEPAGE+y] << flush;
		
		file.close();
	}
// MOD END 
//*************************


	void killProcess()
	{	
		for (int y = 0; y < 2; y++)
		{
			for (int x = 0; x < TAILLE/TAILLEPAGE; x++)		
				if(PageTable[y][x]->getCadre() != FAULT)
					TableDesCadres[PageTable[y][x]->getCadre()] = NULL;
		}
	}

	void run()
	{
		//////////////////////////////////////////////////
		// Resolce return:
		// return (cadre != FAULT ? 
		//		PageTable[segment][page]->getCadre()*TAILLEPAGE + deplacement : 
		//		FAULT);
		//////////////////////////////////////////////////

		CHAR newPC = resolve(PC,0);

		char no		= (RAM[newPC] & 224) >> 5;
		CHAR data	= (RAM[newPC] & 31);
	
		CHAR adresseProg = resolve(data,0);	// resolve retourne adresse memoire physique totale
		CHAR adresseData = resolve(data,1);

		MiseAJour(no, data, adresseProg, adresseData);

		if (State != 'E') return;		// Pas exécutable
		switch (no)
		{
		case 0	: // CPY  valeur	: Copie valeur dans le registre.
				Registre = data;
				PC++;
				break;
		case 1	: // RED [adresse]	: Copie adresse dans le registre.
				Registre = RAM[adresseData];
				PC++;
				break;
		case 2	: // WRT [adresse]	: Copie le registre dans adresse.
				RAM[adresseData] = Registre;
				PC++;
				break;
		case 3	: // ADD [adresse]	: Additionne la valeur dans adresse au registre.
				Registre += RAM[adresseData];
				PC++;
				break;
		case 4	: // SUB  adresse	: Soustrait la valeur dans adresse au registre.
				Registre -= RAM[adresseData];
				PC++;
				break;
		case 5	: // JMP  adresse	: Effectue un saut à l'adresse dans le registre et copie adresse dans le registre.
				PC = Registre;
				Registre = data;
				break;
		case 6	: // JEQ  adresse	: Effectue un saut (jump) à adresse si la valeur du registre est égale à 0.
				if (Registre == 0)	PC = data;
				else				PC++;
				break;
		case 7	: //INT valeur	Effectue l'interruption valeur.
				if (data == 0)
				{
					adresseData = resolve(Registre & 31, 1);
					cout << (RAM[adresseData] != 0 ? (char)(';' + RAM[adresseData]) : ' ');
				}
				if(data==1) State = 'D';
				PC++;
				break;
		}
	}

	void afficherRAM(bool entete = true)
	{
		if (entete)
		{
			cout << endl;
			for (int x = 0; x < TAILLE*2 / 4; x++)
			{
				if(TableDesCadres[x] != NULL)
				{
					cout << (int)(x%10) << flush;
					cout << ":" << flush;
					cout << TableDesCadres[x]->getProcess() << flush;
					cout << (int)TableDesCadres[x]->getSegment() << flush;					
					cout << " ";
				}
				else
					cout << "-" << (x % 10) << "-- ";
			}
		}
		for (int x = 0; x < TAILLE; x++)
		{
			if (x % 4 == 0 && x != 0) cout << " ";
			cout << RAM[x] % 10 ;
		}

		for (int x = TAILLE; x < 2*TAILLE ; x++)
		{
			if (x % 4 == 0 && x != TAILLE) cout << " ";
			cout << (RAM[x] != 0 ? (char)(';' + RAM[x]) : '_');	
		}
		cout << endl;
	}
};		//Fin CProcesseur


class CProcess
{
	static int NoProcess;
	char	ID;
	CHAR	PC;
	char	State;
	CHAR	Registre;

	CTableEntry*** Table;

public:
	CProcess(char id, char state, char* fileName)
	{
		ID			= id;
		PC			= 0;
		State		= state;
		Registre	= 0;
	
		Table = new CTableEntry**[2];
		for (int y = 0; y < 2; y++)
		{
			Table[y] = new CTableEntry*[TAILLE/TAILLEPAGE];
			for (int x = 0; x < TAILLE/TAILLEPAGE; x++)		
				Table[y][x]= new CTableEntry(y,						// segment 
											NoProcess*64+y*32+x*TAILLEPAGE,// NoSwap
											FAULT,					// pas de cadre		//Mise à jour table des pages
											1 |						// read			(1 = read only, 3= rw,5 = rx)
											(y==1 ? 2 : 0) |			// write
											(y!=1 ? 4 : 0),			// execute
											id );					// id				
		}
		prepareSwap(fileName);
		NoProcess ++;
	}

	~CProcess()
	{
		for (int y = 0; y < 2; y++)
		{
			for (int x = 0; x < TAILLE/TAILLEPAGE; x++)
				delete Table[y][x];
			delete Table[y];
		}
		delete Table;
	}

	void prepareSwap(char* fileName)
	{
		ifstream fileIn(fileName);
		if (fileIn.is_open() == false) 
		{
			cout  << "fichier introuvable" << endl;
			exit(-1);
		}

		ofstream fileOut("Swap.cpp", ios::app | ios::binary);		
		fileOut.seekp(NoProcess * 64);
		int compte = 0; 
		while(!fileIn.eof())
		{
			compte ++;
			int tamp1;
			fileIn >> tamp1;
			CHAR tamp2 = (CHAR)tamp1;
			fileOut << tamp2;
		}
		fileOut.close();
		fileIn.close();	  
	}

	char retID()				{	return ID;			}
	char retPC()				{	return PC;			}
	char retState()				{	return State;		}
	char retRegisre()			{	return Registre;	}
	CTableEntry*** retTable()	{	return Table;	}

	void setID(char id)				{	ID = id;			}
	void setPC(CHAR pc)				{	PC = pc;			}
	void setState(char state)		{	State = state;		}
	void setRegisre(CHAR registre)	{	Registre = registre;}
};


int CProcess::NoProcess = 0;


CProcess* getNewProcess();
void setActiveProcess(CProcess *process);
void executerUneInstruction(CProcess* process);
CHAR getNextProcess(CHAR nbrProcess);


const int NombreProcess = 12;
CProcesseur CPU;
CProcess** P = new CProcess*[NombreProcess];


int main()
{
	ofstream fileOut("Swap.cpp", ios::trunc);
	fileOut.close();

	CProcess** P = new CProcess*[NombreProcess];
	for (int x = 0; x < NombreProcess; x++) P[x] = NULL;
	
	int IterMax = -1;
	while (IterMax++ < 50000)
	{ 
		//CPU.afficherRAM();
		int No = getNextProcess(NombreProcess);
		if (P[No] == NULL)	P[No] = getNewProcess();
		else
		{
			switch (P[No]->retState())
			{
			case 'E' :		// executer
				executerUneInstruction(P[No]);
				break;
			case 'D' :		// kill process
				cout << "<" << P[No]->retID() << ">" << endl;
				setActiveProcess(P[No]);
				CPU.killProcess();
				delete P[No];
				P[No] = NULL;
				break;
			}
		}
	}
}

CProcess* getNewProcess()
{
	static int NoName=0;
	
	int type = std::rand()%4;
	CHAR name;
	if('A' + NoName <= 'Z')
	{
		NoName++;
		name = (CHAR)('A'+NoName);
	}
	else
	{
		name = 'A' + (NoName = 0);
	}
	 //= ( ? (CHAR)('A'+NoName++) : 'A' + (NoName = 0)); // Hi Hi Hi
	switch (type)
	{
		case 0 : return new CProcess(name, 'E', "PrintUnMot.txt");	break;
		case 1 : return new CProcess(name, 'E', "Division.txt");	break;
		case 2 : return new CProcess(name, 'E', "Print20A.txt");	break;
		case 3 : return new CProcess(name, 'E', "ViveLaVie.txt");	break;
	}
	return NULL; // arrive jamais en principe
}


void setActiveProcess(CProcess* process)
{
	CPU.setTable(process->retTable());
	CPU.setState(process->retState());			
	CPU.setRegistre(process->retRegisre());
	CPU.setPC(process->retPC());
}


void executerUneInstruction(CProcess* process)
{
	setActiveProcess(process);
		
	CTableEntry* page = NULL;
	while ((page = CPU.getPageFault()) != NULL)
	{
		CPU.loadPage(page);
	}
	CPU.run();

	process->setPC(CPU.retPC());
	process->setRegisre(CPU.retRegistre());
	process->setState(CPU.retState());
}


CHAR getNextProcess(CHAR nbrProcess)
{
	static int numero = 0;
	return numero++ % nbrProcess;
}


