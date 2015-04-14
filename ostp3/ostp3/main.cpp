#include "os.h"
#include "fonctions.h"
#include <cstdio>
#include <ctime>

//Auteurs: Johnny GU�rin et Brian Fillion
//Date de remise: 14 avril 2015

// ON travaille avec des string pour utiliser les fonctions de l'objet. Selon le site http://www.cplusplus.com/reference/string/string/,
// string = char*, avec des fonctions suppl�mentaires.

using namespace std;

int main()
{
	/**/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Initialisation des composants (cr�ation HD.DH, catalogue, fat et stream d'�criture)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Os* operatingSystem;
	operatingSystem = new Os();
	cout << "Disque dure cr�eer (HD.DH), ainsi que le simulateur de systeme d'exploitation" << endl << endl;
	string buffer = "";

	//Pour le timer
	clock_t timer;
	double tempsEcoule;
	
	afficherHD();
	cout << "Liste des fichiers:" << endl;
	afficherListeFichier(operatingSystem);	
	cout << "Traitement de fonctions aleatoires,reaffichage dans 5 secondes ..." << endl;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Boucle infini du programme, conform�ment aux demandes de la sp�cification
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	timer = clock();

	//for(int i = 0; i < 5000; i++)
	while(true)
	{
		tempsEcoule = (clock() - timer) / (double) CLOCKS_PER_SEC;

		if(tempsEcoule < 1.2)	//On affiche � tous les 5 secondes. Avec visual studio: <5, mais plus lent sous sunense, donc 0.5.
		{
			actionAleatoire(operatingSystem);
			//cout << "Action al�atoire, temps ecoul�:" << tempsEcoule << endl;
		}

		else // Sinon on effectue une action al�atoire
		{
			afficherHD();
			cout << "Liste des fichiers:" << endl;
			afficherListeFichier(operatingSystem);	
			cout << "Traitement de fonctions aleatoires, reaffichage dans 5 secondes ..." << endl;
			timer = clock();
		}

	}
	return 0;
}