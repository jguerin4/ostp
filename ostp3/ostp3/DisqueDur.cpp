#include "DisqueDur.h"




DisqueDur::DisqueDur()
{
	streamHDDH = new ofstream("HD.DH",  ios::out);

	for (int j = 0; j < 256; j++)
	{
		for (int i = 0 ; i < 64 ; i++)	//Initialisation de HD.DH (data) avec 64 * 256 => "0"
		{
			
			*streamHDDH << "0";
		}
		*streamHDDH << "\n";
		catalogue[j] =new elementCatalogue();
		catalogue[j]->filesize = 0;
		catalogue[j]->indexFirstBlock = 255;

	}
	streamHDDH->close();	

}


DisqueDur::~DisqueDur()
{
	for (int j = 0; j < 256; j++)
	{
		delete catalogue[j];
	}

	delete streamHDDH;
}

string DisqueDur::readBlock(unsigned char numeroBlock)
{
	char * buffer = new char[64];
	ifstream HDDH("HD.DH", ios::binary);
	if(HDDH)
	{
		HDDH.seekg(numeroBlock*66); //66 parce que (64 char + \n)
		HDDH.read(buffer,64);
		HDDH.close();
		
		string data = (string)buffer;
		data = data.substr(0,64);
		return data;

	}
	else
	{
		cout << " Impossible d'ouvrir le fichier (readBlock)" << endl;
		exit(-1);
	}

}
void DisqueDur::writeBlock(unsigned char numeroBlock, string tampEcriture)
{
		
	string data[256];

	//copie le contenu de HD.DH dans data

	ifstream HDDH("HD.DH", ios::in);
    string buffer = "";
	short i = 0;
	if(HDDH)  
    {       
        // instructions
		while(getline(HDDH, buffer))
		{
			if(i <= 255)
			{
				data[i] = buffer;
				while(data[i].size() != 64)
				{
					if( data[i].size() <= 64)
					{
						data[i].append("0");
					}
					else
					{
						 exit(-1);
					}
				}

			}
			else
				break;
			i++;
		}
        HDDH.close();  // on ferme le fichier
    }

	else  // l'ouverture �choue
	{
		exit(-1);
	}


		
	//On s'assure que la longeur de tampEcriture est de 64, sinon on rempli avec des "0"

	//R��crit le fichier avec la modification
	data[(const unsigned char)numeroBlock] = tampEcriture;
	streamHDDH->open("HD.DH",  ios::out);
	for (int line = 0; line < 256; line++)
	{
		*streamHDDH << data[line];
		*streamHDDH << "\n";
	}
	streamHDDH->close();

}

void DisqueDur::createElementCatalogue(int i,elementCatalogue* element)
{
	catalogue[i] = element;
}

elementCatalogue* DisqueDur::getElementCatalogue(int i)
{
	return catalogue[i]; 
}
