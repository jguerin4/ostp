Bonjour, nous sommes:

Brian Fillion et Johnny Gu�rin

Voici comme nous avons impl�ment� notre NFU:

Nous avons d'abord modifi� l'algorithme pour recevoir un cadre de page libre lorsqu'on charge une page.
Celui-ci consiste a rechercher le cadre ayant le compteur R le plus bas.
Si on atteint un cadre NULL, on associe notre page a ce cadre. Sinon, on s�lectionne en priorit� les pages n'ayant 
pas �t� modifi�s. Finalement, on v�rifie parmi le compteur R. Tout ces changements sont situ�s dans la fonction getCadreLibre.

� chaque fois qu'on r�f�re une page, on fait un d�calage � droite (/=2) suivi de R += MAX_UNSIGNED_INT/2 au cadre que l'on utilise
 et un d�calage � droite (R /=2) aux autre cadres. On modifie le bit M lorsqu'une page � �t� modifi�. Tout ces changements se produisent
dans la fonction MiseAJour.

Lorsque l'on enregistre une page qui � �t� modifi� pour utiliser son cadre, on remet son bitR � 0 correctement.

La protection des pages interdit bien d'ex�cuter des pages des donn�es ou modifier des pages de code. Pour ce, nous v�rifier , avant de mettre � jour les bits M et R 
d'une page ainsi que son "age de r�f�rencement", nous v�rifions quel est sa protection et quittons le programme avec un message d'erreur si une action interdite est tent�.
Le fait que le programme ne s'arr�te pas prouve que les bonnes pages sont biens charg�s, puisque sinon le programme essaierait forc�ment une action interdite si ce n'�tait
pas le cas � un moment ou � un autre.

Nous avons r�ussi � tout faire ceci sans AUCUNEMENT modifier le code dans le processeur en dehors de la zone d�limit� par MOD START et MOD END.

Le serveur Sunens n'�tait pas disponible lors de la remise, nous avons donc g�n�r� un a.out sur une vm linux pour que nous puissions le remettre par email .
(Via le site c9.io, qui est vraiment int�ressant!)

