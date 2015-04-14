#include <iostream>
#include "Os.h"
#include "fonctions.h"
#include <cstdio>
#include <ctime>

//Auteurs: Johnny GU�rin et Brian Fillion
//Date de remise: 14 avril 2015

// ON travaille avec des string pour utiliser les fonctions de l'objet. Selon le site http://www.cplusplus.com/reference/string/string/,
// string = char*, avec des fonctions suppl�mentaires.

using namespace std;

void main()
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

	while(true)
	{
		tempsEcoule = (clock() - timer) / (double) CLOCKS_PER_SEC;

		if(tempsEcoule < 5.0)	//On affiche � tous les 5 secondes
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
}
