#pragma once
#include <string>
#include <fstream>

using namespace std;
struct elementCatalogue{	// Le catalogue sert à indiquer le premier index d'un fichier dans la FAT-8
	string fileName;
	unsigned char indexFirstBlock;
};

class DisqueDur
{
public:
	DisqueDur(void);
	~DisqueDur(void);

	void readBlock(unsigned char, string *tampLecture);
	void writeBlock(unsigned char, string *tampEcriture);

private:
	ofstream streamHDDH;
	elementCatalogue catalogue[256];
};

