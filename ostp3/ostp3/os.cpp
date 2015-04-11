#include "os.h"

#include <iostream>
Os::Os(void)
{
	hd = new DisqueDur();

	for(int i =0;i<256;i++)
	{
		fat[i] = 0;
	}
}


Os::~Os(void)
{
	delete hd;
}

void Os::read(string nomFichier, CHAR position, CHAR nombreCaractere, string* tampLecture)
{
	CHAR beginIndex =255;
	for(int i=0;i<256;i++)
	{
		if(hd->getElementCatalogue(i)->fileName == nomFichier)
		{
			beginIndex = hd->getElementCatalogue(i)->indexFirstBlock;
			break;
		}
	}

	if(beginIndex == 255)
	{
		cout<<"pas de fichier trouver dans le catalogue"<<endl;
	}

	if (position + nombreCaractere > hd->getElementCatalogue(beginIndex)->filesize)
	{
		nombreCaractere = hd->getElementCatalogue(beginIndex)->filesize - position;
	}

	string* tempRead = new string();
	*tempRead = "";
	CHAR positionItr=0;
	//set position
	while(positionItr != position)
	{
		//division entiere pour voir si sur le meme bloc
		if(position/64 != positionItr/64)
		{
			beginIndex = fat[beginIndex];
			positionItr +=64;
		}
	}

	int charRead=0;
	//get right char amount
	while(charRead != nombreCaractere)
	{
		*tempRead = hd->readBlock(beginIndex);
		string* blockString = new string();

		if(nombreCaractere- charRead <=64)
		{
			*blockString =tempRead->substr(nombreCaractere- charRead);
		}
		else if(charRead ==0)
		{
			*blockString =tempRead->substr(position-positionItr);
		}
		else
		{
			*blockString =*tempRead;
		}

		*tampLecture += *blockString;
		charRead += blockString->size();

		if(charRead > nombreCaractere)
		{
			cout<<"os read chie, on lie trop"<<endl;
		}
	}
}

CHAR Os::getBlocLibre()
{
	for(int i=0;i<256;i++)
	{
		if(fat[i]==0)
		{
			return i;
		}
	}

	return 255;
}

void Os::write(string nomFichier, CHAR position, CHAR nombreCaractere, string* tampLecture)
{
	CHAR beginIndex =255;
	for(int i=0;i<256;i++)
	{
		if (hd->getElementCatalogue(i)->fileName == nomFichier)
		{
			beginIndex = hd->getElementCatalogue(i)->indexFirstBlock;
			break;
		}
	}

	if(beginIndex == 255)
	{
		//create new file
		CHAR blocLibre = getBlocLibre();
		elementCatalogue toAdd = elementCatalogue();
		toAdd.fileName = nomFichier;
		toAdd.filesize = position + nombreCaractere;
		toAdd.indexFirstBlock = blocLibre;
		hd->createElementCatalogue(blocLibre, &toAdd);

		beginIndex = blocLibre;
	}

	string* tempWrite = new string();
	CHAR positionItr=0;
	//set position
	while(positionItr != position)
	{
		//division entiere pour voir si sur le meme bloc
		if(position/64 != positionItr/64)
		{
			beginIndex = fat[beginIndex];
			positionItr +=64;
		}
	}

	int charRead=0;
	//get right char amount
	while(charRead != nombreCaractere)
	{
		*tempWrite = hd->readBlock(beginIndex);
		string* blockString = new string();

		if(nombreCaractere- charRead <=64)//dernier bloc a ecrire dedans
		{
			*blockString =tempWrite->substr(nombreCaractere- charRead);
		}
		else if(charRead ==0)// premier bloc a ecrire dedans
		{
			*blockString =tempWrite->substr(position-positionItr);
		}
		else//bloc entre les deux
		{
			*blockString =*tempWrite;
		}

		*tampLecture += *blockString;
		charRead += blockString->size();

		if(charRead > nombreCaractere)
		{
			cout<<"os read chie, on lie trop"<<endl;
		}
	}
}
void Os::deleteEOF(string nomFichier, CHAR position)
{

}
