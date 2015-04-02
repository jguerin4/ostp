#include "DisqueDur.h"


DisqueDur::DisqueDur()
{
	streamHDDH = ofstream("HD.DH", ofstream::out);

	for (int i = 0 ; i < 64*256 ; i++)
	{
		streamHDDH << "0";
	}

	streamHDDH.close();	

	

}


DisqueDur::~DisqueDur()
{
}

void DisqueDur::readBlock(unsigned char numeroBlock, string *tampLecture)
{

}
void DisqueDur::writeBlock(unsigned char numeroBlock, string *tampEcriture)
{
	
}