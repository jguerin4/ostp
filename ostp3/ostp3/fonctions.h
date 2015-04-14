#include "time.h"
#include "stdlib.h"


void ecritureAleatoire(Os* operatingSystem)
{
	int randomAlphabet = rand() % 26 +97;
	int longueurMot = rand()%400;

	char randomChar[450];	// Initialisation du tableau avec que des lettres �quivalente al�atoire
	fill(randomChar,randomChar+(int)longueurMot,randomAlphabet); // remplit randomChar par une r�p�tition de lettre.
	string randomString = randomChar;
	randomString = randomString.substr(0,longueurMot); // Pour regler rapidement un probl�me d'overflow

	char fileName[5] = {randomAlphabet,46,116,120,116};	// revient � [a-z].txt
	string newFileName = fileName;
	newFileName = newFileName.substr(0,5);	//M�me raison

	operatingSystem->write(newFileName, 0,longueurMot,&randomString);

	//cout << "randomChar: " << randomString << endl;
	//cout << "new file name: " << newFileName << endl;
}

void afficherHD()
{
	cout << "Contenu du disque dur:" << endl;
	ifstream hardDriveStream;
	hardDriveStream.open("HD.DH");
	char output[70];

	if (hardDriveStream.is_open())
	{
		 while (!hardDriveStream.eof())
		 {

			hardDriveStream >> output;
			cout <<output << endl;
		 }
	}
hardDriveStream.close();
}

void afficherListeFichier(Os* operatingSystem)
{
	for(int i = 0 ; i < 256; i++)
	{
		elementCatalogue* catalogueFAT8 = operatingSystem->getHD()->getElementCatalogue(i);
		if(catalogueFAT8->filesize != 0 && catalogueFAT8->fileName != "")
		{
			
			cout << "Fichier #" << i << " : " << catalogueFAT8->fileName;
			operatingSystem->showFileBlocks(catalogueFAT8->fileName);
		}
	}
	cout << endl << "--------------- Fin affichage ---------------" << endl;
}

void supprimerFichierAleatoire(Os* operatingSystem, int position)
{
	int randomAlphabet = rand() % 26 +97;
	char fileName[5] = {randomAlphabet,46,116,120,116};	// revient � [a-z].txt
	string newFileName = fileName;
	newFileName = newFileName.substr(0,5);	//Pour regler rapidement un probl�me d'overflow. 

	operatingSystem->deleteEOF(newFileName,position);	//Position 0 delete le fichier (Pas lit�ralement, on
												// ne fait qu'indiqu� dans la FAT que je fichier � �t� supprim�

}


void actionAleatoire(Os* operatingSystem)
{

	srand (clock());
	int writeOrDelete = rand() %5;
	int positionAleatoire;

	switch(writeOrDelete)
	{
	case 0:	//une chance sur 4 de supprimer un fichier au complet
		supprimerFichierAleatoire(operatingSystem,0);
		break;

	case 1:
		positionAleatoire = rand() % 63 + 1;	//Une chance sur 4 de supprimer un fichier partiellement
		supprimerFichierAleatoire(operatingSystem,positionAleatoire);
		break;

	default:	//Une chance sur deux d'�crire/append un nouveau fichier
		ecritureAleatoire(operatingSystem);
		break;
	}

}