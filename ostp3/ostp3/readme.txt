Système d'exploitation, laboratoire #3

Auteur: Johnny GUérin et Brian Fillion
Date de remise: 14 Avril 2015

serveur de dépot: sunense.uqac.ca (port 23)
Username: ex28012
Password: sgl733

Description du programme:

Ce programme simule le système de fichier d'un système d'exploitation.

Ses composants sont principalement:
OS : Fonctions read, write et deleteEOF, qui utilisent l'interface du disque dur, le catalogue et la FAT8 (tableau de longueur 256)
	 afin de respecter leurs fonctions. Les 3 fonctions sont conformes aux spécifications demandés
Disque dur: Classe qui interface le disque dur, permet de lire et d'écrire un bloc un par un. Utilisé par l'OS afin 
			de respecter la spécification.
main: Affichage du disque dur et de la liste de fichier (et de leurs bloc(s)) , afin de tester l'application.

Note: - HD.DH et un fichier de 256*64 caractères pour représenter une disque dur. Afin de faciliter sa lisibilité, nous avons ajouter des sauts de lignes à 
	    chaque bloc. 
		
	  - Information sur la FAT8:
		Desciption: Tableau de longueur 256 qui contient des unsigned char ([0-255]).
		255: Fin de fichier (représenté par -1 dans les notes de cours)
		0: NULL, bloc libre. C'est pourquoi la première ligne dans HD.DH n'est jamais utilisé , puisque l'on à besoin de son index ici.
		Autre: index du prochain bloc à lire
		
	  - Information sur le catalogue:
		Description: un tableau de longueur 256. Chaque élément est une structure qui comporte le nom du fichier, sa taille ainsi que l'index du premier bloc.
		