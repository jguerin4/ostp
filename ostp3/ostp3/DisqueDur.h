#pragma once
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct elementCatalogue{	// Le catalogue sert à indiquer le premier index d'un fichier dans la FAT-8
public :
	string fileName;
	unsigned char indexFirstBlock;
	unsigned char filesize;
};

class DisqueDur
{
public:
	DisqueDur(void);
	~DisqueDur(void);

	string readBlock(unsigned char numéroBlock);
	void writeBlock(unsigned char numeroBlock, string tampEcriture);
	elementCatalogue* getElementCatalogue(int i);
	void createElementCatalogue(int i,elementCatalogue* element);

private:
	ofstream streamHDDH;
	elementCatalogue *catalogue[256];
};
