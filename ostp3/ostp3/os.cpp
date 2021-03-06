#include "os.h"

#include <iostream>
Os::Os(void)
{
	hd = new DisqueDur();

	fill(fat,fat+256,0);
}


Os::~Os(void)
{
	delete hd;
}

DisqueDur* Os::getHD()
{
	return hd;
}

void Os::read(string nomFichier, int position, int nombreCaractere, string* tampLecture)
{
	CHAR beginIndex = 0;
	//on cherche le premier index dans le catalogue de fichier
	for (int i = 1; i<256; i++)
	{
		if (hd->getElementCatalogue(i)->fileName == nomFichier)
		{
			beginIndex = hd->getElementCatalogue(i)->indexFirstBlock;
			if (hd->getElementCatalogue(i)->filesize < position + nombreCaractere)
			{
				if (hd->getElementCatalogue(i)->filesize < position)
				{
					cout << "on demande de lire en dehors du fichier size = " << (int)hd->getElementCatalogue(i)->filesize << " on veut lire jusqua: " << (int)(position + nombreCaractere) << endl;
				}
				else
				{
					nombreCaractere = hd->getElementCatalogue(i)->filesize - position;
				}
				
			}
			break;
		}
	}

	string* tempRead = new string();
	int fileDelay =0;
	//on se place dans le bloc ou se trouve Position
	while (fileDelay + 64 < position)
	{
		if (beginIndex != 255)
		{
			beginIndex = fat[beginIndex];
			fileDelay += 64;
		}
		else
			break;

	}

	int blocIndex = beginIndex;
	int charRead = 0;
	//on lit ce quil faut dans les blocs
	while (true)
	{
		*tempRead= hd->readBlock(blocIndex);
		string* rightBlockString = new string();
		string* leftBlockString = new string();

		if (charRead == 0)// premier bloc a lire dedans
		{
			*leftBlockString = tempRead->substr(0, position%64);

			if (nombreCaractere - charRead <= 64)
			{
				*tempRead = tempRead->substr(position, nombreCaractere);
			}
			else
			{
				*tempRead = tempRead->substr(position%64, 64-position%64);
			}
		}
		else if (nombreCaractere - charRead <= 64)//dernier bloc a ecrire dedans
		{
			*tempRead = tempRead->substr(0, nombreCaractere - charRead);
		}

		charRead += tempRead->size();

		if (charRead > nombreCaractere)
		{
			cout << "os read disfonctionnel, on lie trop" << endl;
		}

		cout << "reading bloc " << blocIndex << ":" << *tempRead << endl;

		if (charRead >= nombreCaractere)
		{
			break;
		}
		else
		{
			blocIndex = fat[blocIndex];
			if (blocIndex == 255)
			{
				cout << "end of file "<<endl;
				break;
			}
				
		}
	}
}

CHAR Os::getBlocLibre()
{
	for(int i=1;i<256;i++)
	{
		if(fat[i]==NULL)
		{
			return i;
		}
	}
	cout<<"LE DUR EST PLEIN, AUTO DESTRUCTION"<<endl;
	exit(-1);
	return NULL;
}

CHAR Os::CreateFile(string name, int size)
{
	//create new file
	CHAR blocLibre = getBlocLibre();
	elementCatalogue* toAdd = new elementCatalogue();
	toAdd->fileName = name;
	toAdd->filesize = size;
	toAdd->indexFirstBlock = blocLibre;
	hd->createElementCatalogue(blocLibre, toAdd);
	fat[blocLibre] = 255;//fin de fichier

	return blocLibre;
}

CHAR Os::ExtendFile(CHAR blocIndex)
{
	//on avance de blocs dans la fat
	if (fat[blocIndex] == 255)//pus de blocs pour ce fichier
	{
		fat[blocIndex] = getBlocLibre();
		fat[fat[blocIndex]] = 255;//met la nouvelle fin de fichier
	}

	return fat[blocIndex];
}

void Os::write(string nomFichier, int position, int nombreCaractere, string* tampEcriture)
{
	CHAR beginIndex =0;
	//on cherche le premier index du fichier dans le catalogue
	for(int i=1;i<256;i++)
	{
		if (hd->getElementCatalogue(i)->fileName == nomFichier)
		{
			beginIndex = hd->getElementCatalogue(i)->indexFirstBlock;
			if (hd->getElementCatalogue(i)->filesize < position + nombreCaractere)
			{
				hd->getElementCatalogue(i)->filesize = position + nombreCaractere;
			}
			break;
		}
	}

	//le fichier est pas dans le catalogue donc on le cr�e
	if(beginIndex == 0)
	{
		beginIndex = CreateFile(nomFichier, position + nombreCaractere);
	}

	string* tempWrite = new string();
	CHAR fileDelay=0;
	//on se place dans le bloc ou se trouve Position
	while (fileDelay + 64 < position)
	{
		if (beginIndex != 255)
		{
			beginIndex = fat[beginIndex];
		}
		else
		{
			beginIndex = ExtendFile(beginIndex);
		}
		fileDelay += 64;
	}

	int blocIndex = beginIndex;
	int charRead=0;
	int firstReadDelay = 0;
	//on ecrit ce quil faut dans les blocs
	while (true)
	{
		string tampBlocZone = tampEcriture->substr(charRead - ((charRead + firstReadDelay) % 64), 64);
		*tempWrite = hd->readBlock((CHAR)blocIndex);
		string* rightBlockString = new string();
		string* leftBlockString = new string();
		if (charRead == 0)// premier bloc a ecrire dedans
		{
			*leftBlockString = tempWrite->substr(0, position%64);
			firstReadDelay = leftBlockString->size();
			if (nombreCaractere - charRead <= 64)
			{
				*rightBlockString = tempWrite->substr(position - fileDelay, 64 - leftBlockString->size() - tampBlocZone.size());
			}
			*tempWrite = tampBlocZone.substr(0, 64 - firstReadDelay);

			

			charRead += tempWrite->size();
			tampBlocZone = *leftBlockString + *tempWrite + *rightBlockString;
			//*tampEcriture = tampEcriture->substr(position - positionItr,64);
		}
		else if (nombreCaractere - charRead <= 64)//dernier bloc a ecrire dedans
		{
			*rightBlockString = tempWrite->substr(nombreCaractere - charRead);
			charRead += tampBlocZone.size();
			tampBlocZone += *rightBlockString;
		}
		else//bloc entre les deux
		{
			charRead += tampBlocZone.size();
		}

		if (charRead > nombreCaractere)
		{
			cout << "os write �disfonctionnel, on lie trop" << endl;
		}
		hd->writeBlock((CHAR)blocIndex, tampBlocZone);
		if (charRead >= nombreCaractere)
		{
			break;
		}
		else
		{
			blocIndex = ExtendFile(blocIndex);
		}
	}

}

void Os::showFileBlocks(string nomFichier)
{
	CHAR index = 0;
	for (int i = 1; i<256; i++)
	{
		if (hd->getElementCatalogue(i)->fileName == nomFichier)
		{
			index = hd->getElementCatalogue(i)->indexFirstBlock;
			break;
		}
	}
	cout<<"Bloc: ";
	while(index != 255)
	{
		cout<<(int)index<<", ";
		index = fat[index];
	}

	cout<<endl;
	return;
}

void Os::deleteEOF(string nomFichier, int position)
{
	CHAR beginIndex = 0;
	for (int i = 1; i<256; i++)
	{
		if (hd->getElementCatalogue(i)->fileName == nomFichier)
		{
			beginIndex = hd->getElementCatalogue(i)->indexFirstBlock;
			hd->getElementCatalogue(i)->filesize = position;
			break;
		}
	}

	int fileDelay = 0;
	while (fileDelay + 64 < position)
	{
		if (beginIndex != 255)
		{
			beginIndex = fat[beginIndex];
			fileDelay += 64;
		}
		else
		{
			cout << "la position est plus grande que le fichier" << endl;
			return;
		}
	}

	string* tempRead = new string();
	int blocIndex = beginIndex;
	
	//delete right char amount
	*tempRead = hd->readBlock(blocIndex);
	string* leftBlockString = new string();
	*leftBlockString = tempRead->substr(0, position % 64);

	hd->writeBlock(blocIndex, *leftBlockString + string(64 - position % 64, '0'));

	int oldIndex = fat[blocIndex];
	fat[blocIndex] = 255;
	blocIndex = oldIndex;
	

	while (true)
	{
		if (blocIndex != 255)
		{
			oldIndex = fat[blocIndex];
			fat[blocIndex] = NULL;
			blocIndex = oldIndex;
		}
		else
		{
			break;
		}
	}

}
