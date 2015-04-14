#include "time.h"
#include "stdlib.h"

void actionAleatoire()
{
	

}

void ecritureAleatoire(Os* operatingSystem)
{
	int randomAlphabet = rand() % 26 +97;

	char randomChar[3];	// Initialisation du tableau avec que des lettres équivalente aléatoire
	fill(randomChar,randomChar+3,randomAlphabet); // remplit randomChar par une répétition de lettre.
	string randomString = randomChar;
	randomString = randomString.substr(0,3); // Pour regler rapidement un problème d'overflow

	char fileName[5] = {randomAlphabet,46,116,120,116};	// revient à [a-z].txt
	string newFileName = fileName;
	newFileName = newFileName.substr(0,5);	//Même raison

	operatingSystem->write(newFileName, 0,3,&randomString);

	//cout << "randomChar: " << randomString << endl;
	//cout << "new file name: " << newFileName << endl;
}

void afficherHD()
{
	cout << "Contenu du disque dur:" << endl;
	ifstream hardDriveStream;
	hardDriveStream.open("HD.DH");
	char output[70];

	if (hardDriveStream.is_open())
	{
		 while (!hardDriveStream.eof())
		 {

			hardDriveStream >> output;
			cout <<output << endl;
		 }
	}
hardDriveStream.close();
}

void afficherListeFichier()
{

}

void supprimerFichierAleatoire(Os* operatingSystem)
{

}

void supprimerEOFAleatoire(Os* operatingSystem)
{

}