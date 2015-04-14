#include <iostream>
#include "Os.h"
#include "fonctions.h"
#include <cstdio>
#include <ctime>


//Auteurs: Johnny GU�rin et Brian Fillion
//Date de remise: 14 avril 2015

// ON travaille avec des string pour utiliser les fonctions de l'objet. Selon le site http://www.cplusplus.com/reference/string/string/,
// string * = char*



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
	int writeOrDelete;

	//Pour le timer
	clock_t timer;
	double tempsEcoule;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Phase de test #1: Cr�ation d'un simple fichier texte (testPrimaire.txt) et sa lecture
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	cout << "Phase de test #1: write+read simple qui rentre dans un bloc: " << endl;
	string test = "testaaaaaaaaaaaaaaaaaaaaaaa";

	operatingSystem->write("testPrimaire.txt", 0, test.size(), &test);		//Cr�e le fichier s'il n'existe pas (index premier block = 0)
																			// Position = num�ro de d�but dans la ligne
	operatingSystem->read("testPrimaire.txt", 0, 2000, &buffer);
	cout << " Buffer: " << buffer << endl << endl;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Phase de test #2: Cr�ation d'un fichier al�atoire
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout << "Phase de test #2: write+read simple d'un fichier avec des lettres al�atoires: " << endl;



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Phase de test #3: Affichage du disque dur apr�s les tests
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	afficherHD();	//d�finit dans fonctions.h, affiche HD.DH

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Boucle infini du programme, conform�ment aux demandes de la sp�cification
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	timer = clock();

	while(true)
	{
		tempsEcoule = (clock() - timer) / (double) CLOCKS_PER_SEC;

		if(tempsEcoule >= 5.0)	//On affiche � tous les 5 secondes
		{
			afficherHD();
			cout << "Liste des fichiers:" << endl;
			afficherListeFichier();	//RESTANT !!!!!!!!!!!!!!!!!!!!!!!!!!!!
			timer = clock();
		}

		else // Sinon on effectue une action al�atoire
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
