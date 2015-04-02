Bonjour, nous sommes:

Brian Fillion et Johnny Guérin

Voici comme nous avons implémenté notre NFU:

Nous avons d'abord modifié l'algorithme pour recevoir un cadre de page libre lorsqu'on charge une page.
Celui-ci consiste a rechercher le cadre ayant le compteur R le plus bas.
Si on atteint un cadre NULL, on associe notre page a ce cadre. Sinon, on sélectionne en priorité les pages n'ayant 
pas été modifiés. Finalement, on vérifie parmi le compteur R. Tout ces changements sont situés dans la fonction getCadreLibre.

À chaque fois qu'on réfère une page, on fait un décalage à droite (/=2) suivi de R += MAX_UNSIGNED_INT/2 au cadre que l'on utilise
 et un décalage à droite (R /=2) aux autre cadres. On modifie le bit M lorsqu'une page à été modifié. Tout ces changements se produisent
dans la fonction MiseAJour.

Lorsque l'on enregistre une page qui à été modifié pour utiliser son cadre, on remet son bitR à 0 correctement.

La protection des pages interdit bien d'exécuter des pages des données ou modifier des pages de code. Pour ce, nous vérifier , avant de mettre à jour les bits M et R 
d'une page ainsi que son "age de référencement", nous vérifions quel est sa protection et quittons le programme avec un message d'erreur si une action interdite est tenté.
Le fait que le programme ne s'arrête pas prouve que les bonnes pages sont biens chargés, puisque sinon le programme essaierait forcément une action interdite si ce n'était
pas le cas à un moment ou à un autre.

Nous avons réussi à tout faire ceci sans AUCUNEMENT modifier le code dans le processeur en dehors de la zone délimité par MOD START et MOD END.

Le serveur Sunens n'était pas disponible lors de la remise, nous avons donc généré un a.out sur une vm linux pour que nous puissions le remettre par email .
(Via le site c9.io, qui est vraiment intéressant!)

