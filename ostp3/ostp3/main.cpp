#include <iostream>
#include "Os.h"
#include "fonctions.h"
#include <cstdio>
#include <ctime>


//Auteurs: Johnny GUérin et Brian Fillion
//Date de remise: 14 avril 2015

// ON travaille avec des string pour utiliser les fonctions de l'objet. Selon le site http://www.cplusplus.com/reference/string/string/,
// string * = char*



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
	int writeOrDelete;

	//Pour le timer
	clock_t timer;
	double tempsEcoule;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Phase de test #1: Création d'un simple fichier texte (testPrimaire.txt) et sa lecture
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	cout << "Phase de test #1: write+read simple qui rentre dans un bloc: " << endl;
	string test = "testaaaaaaaaaaaaaaaaaaaaaaa";

	operatingSystem->write("testPrimaire.txt", 0, test.size(), &test);		//Crée le fichier s'il n'existe pas (index premier block = 0)
																			// Position = numéro de début dans la ligne
	operatingSystem->read("testPrimaire.txt", 0, 2000, &buffer);
	cout << " Buffer: " << buffer << endl << endl;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Phase de test #2: Création d'un fichier aléatoire
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout << "Phase de test #2: write+read simple d'un fichier avec des lettres aléatoires: " << endl;



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Phase de test #3: Affichage du disque dur après les tests
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	afficherHD();	//définit dans fonctions.h, affiche HD.DH

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Boucle infini du programme, conformément aux demandes de la spécification
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	timer = clock();

	while(true)
	{
		tempsEcoule = (clock() - timer) / (double) CLOCKS_PER_SEC;

		if(tempsEcoule >= 5.0)	//On affiche à tous les 5 secondes
		{
			afficherHD();
			cout << "Liste des fichiers:" << endl;
			afficherListeFichier();	//RESTANT !!!!!!!!!!!!!!!!!!!!!!!!!!!!
			timer = clock();
		}

		else // Sinon on effectue une action aléatoire
		{
			srand (clock());
			writeOrDelete = rand() %5;
			
			switch(writeOrDelete)
			{
			case 0:	//une chance sur 4 de supprimer un fichier au complet
				supprimerFichierAleatoire(operatingSystem);	//RESTANT!!!
				break;

			case 1:
				supprimerEOFAleatoire(operatingSystem);
				break;

			default:
				ecritureAleatoire(operatingSystem);
				break;
			}
		}

	}

	/**/
}
