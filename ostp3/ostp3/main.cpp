#include <iostream>
#include "DisqueDur.h"

// ON travaille avec des string pour utiliser les fonctions de l'objet. Selon le site http://www.cplusplus.com/reference/string/string/,
// string * = char*
using namespace std;

void main()
{
	DisqueDur disqueOS;
	cout << "disque dure créeer, fichier HD.DH aussi" << endl;
	string mot = "salut, ce n'est simplement qu'un mot pour te dire salut";
	 string mot2 = "ihefvbgwiuebfiwbfyiuwbyr";
	disqueOS.writeBlock(1,mot);
	disqueOS.writeBlock(5,mot2);
	string readMe = disqueOS.readBlock(1);

	cout << "Reading : " << readMe << endl;
	
		exit(0);

}
