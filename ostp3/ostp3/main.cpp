#include <iostream>
#include "Os.h"

// ON travaille avec des string pour utiliser les fonctions de l'objet. Selon le site http://www.cplusplus.com/reference/string/string/,
// string * = char*
using namespace std;

void main()
{
	Os* os;
	os = new Os();
	cout << "disque dure créeer, fichier HD.DH aussi" << endl;
	string mot = "salut, ce n'est simplement qu'un mot pour te dire salutadsdsadasaddaadsdassdasddsadsddadaadadadaddsadasdasdasdasdasdasdasdasdasdadadsadasdasdasdasdasdasdasdadasasasdasdadsasdasdads";
	string chien = "woof woof esti";
	string vide = "";
	os->write("chat.txt", 10, mot.size(), &mot);
	//os->write("chien.txt", 0, chien.size(), &chien);
	os->write("chat.txt", 102, chien.size(), &chien);
	
	os->deleteEOF("chat.txt", 90);
	os->read("chat.txt", 0, 2000, &vide);
	system("pause");
	exit(0);

}
