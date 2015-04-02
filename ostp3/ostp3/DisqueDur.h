#pragma once
#include <string>

using namespace std;
class DisqueDur
{
public:
	DisqueDur(void);
	~DisqueDur(void);

	void readBlock(unsigned char, char *tampLecture[64]);
	void writeBlock(unsigned char, char *tampEcriture[64]);


};

