#include <iostream>
#include "DisqueDur.h"

// ON travaille avec des string pour utiliser les fonctions de l'objet. Selon le site http://www.cplusplus.com/reference/string/string/,
// string * = char*
using namespace std;

void main()
{
	DisqueDur* disqueOS;
	disqueOS = new DisqueDur();
	cout << "disque dure créeer, fichier HD.DH aussi" << endl;
	string mot = "salut, ce n'est simplement qu'un mot pour te dire salut";
	 
	disqueOS->writeBlock(1,mot);
	string readMe = disqueOS->readBlock(0);

	cout << "Reading : " << readMe << endl;
	system("pause");
		exit(0);

}
