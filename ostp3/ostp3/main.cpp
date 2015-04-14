#include <iostream>
#include "Os.h"
#include "fonctions.h"
#include <cstdio>
#include <ctime>

//Auteurs: Johnny GUérin et Brian Fillion
//Date de remise: 14 avril 2015

// ON travaille avec des string pour utiliser les fonctions de l'objet. Selon le site http://www.cplusplus.com/reference/string/string/,
// string = char*, avec des fonctions supplémentaires.

using namespace std;

void main()
{
	/**/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Initialisation des composants (création HD.DH, catalogue, fat et stream d'écriture)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Os* operatingSystem;
	operatingSystem = new Os();
	cout << "Disque dure créeer (HD.DH), ainsi que le simulateur de systeme d'exploitation" << endl << endl;
	string buffer = "";

	//Pour le timer
	clock_t timer;
	double tempsEcoule;
	
	afficherHD();
	cout << "Liste des fichiers:" << endl;
	afficherListeFichier(operatingSystem);	
	cout << "Traitement de fonctions aleatoires,reaffichage dans 5 secondes ..." << endl;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Boucle infini du programme, conformément aux demandes de la spécification
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	timer = clock();

	while(true)
	{
		tempsEcoule = (clock() - timer) / (double) CLOCKS_PER_SEC;

		if(tempsEcoule < 5.0)	//On affiche à tous les 5 secondes
		{
			actionAleatoire(operatingSystem);
			//cout << "Action aléatoire, temps ecoulé:" << tempsEcoule << endl;
		}

		else // Sinon on effectue une action aléatoire
		{
			afficherHD();
			cout << "Liste des fichiers:" << endl;
			afficherListeFichier(operatingSystem);	
			cout << "Traitement de fonctions aleatoires, reaffichage dans 5 secondes ..." << endl;
			timer = clock();
		}
	}
}
