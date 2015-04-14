#include <iostream>
#include "Os.h"

// ON travaille avec des string pour utiliser les fonctions de l'objet. Selon le site http://www.cplusplus.com/reference/string/string/,
// string * = char*
using namespace std;

void main()
{/**
	Os* os;
	os = new Os();
	cout << "disque dure créeer, fichier HD.DH aussi" << endl;
	DisqueDur disqueOS;

	string mot2 = "ihefvbgwiuebfiwbfyiuwbyr";
	disqueOS.writeBlock(5,mot2);
	string readMe = disqueOS.readBlock(5);

	cout << "Reading : " << readMe << endl;

	string mot = "salut, ce n'est simplement qu'un mot pour te dire salutadsdsadasdadasasasdasdadsasdasdads";
	string chien = "woof woof esti";
	string vide = "";
	os->write("chat.txt", 10, mot.size(), &mot);
	//os->write("chien.txt", 0, chien.size(), &chien);
	os->write("chat.txt", 102, chien.size(), &chien);
	os->read("chat.txt", 0, 2000, &vide);
	os->deleteEOF("chat.txt", 0);

	os->read("chat.txt", 0, 2000, &vide);

	exit(0);/**/

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Initialisation des composants (création HD.DH, catalogue, fat et stream d'écriture)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Os* operatingSystem;
	operatingSystem = new Os();
	cout << "Disque dure créeer (HD.DH), ainsi que le simulateur de systeme d'exploitation" << endl << endl;
	string buffer = "";
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Phase de test #1: Création d'un simple fichier texte (testPrimaire.txt) et sa lecture
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	string test = "Bonjour. Savez vous que l'important dans la vie est d'être heureux?";
	//test += "Etes-vous heureux? \n SERIEUSEMENT?!?! \n Et bien reveillez-vous!! ";

	operatingSystem->write("testPrimaire.txt", 10, test.size(), &test);		//Crée le fichier s'il n'existe pas (index premier block = 0)
																			// Position = numéro de block dans HD.DH
	operatingSystem->read("testPrimaire.txt", 0, 2000, &buffer);
	cout << " Buffer: " << buffer << endl;


}
