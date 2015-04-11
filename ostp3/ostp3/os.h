#include "DisqueDur.h"
#include <string>

#define CHAR unsigned char

using std::string;
#pragma once
class Os
{
private:
	DisqueDur* hd;
	CHAR fat[256];
public:
	Os(void);
	~Os(void);

	void read(string nomFichier,CHAR position, CHAR nombreCaractere, string* tampLecture);
	void write(string nomFichier,CHAR position, CHAR nombreCaractere, string* tampLecture);
	void deleteEOF(string nomFichier,CHAR position);
	CHAR getBlocLibre();
};

