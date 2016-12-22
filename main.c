#include "cartes.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	
	cartes sabot[6*52]; /*à modifier: 6 ne devra pas être codé en dur*/
	char noms[13] = {'1','2','3','4','5','6','7','8','9','X','V','D','R'};
	int j;
	
	for (j = 0; j < 10; j++) { 	
		sabot[j].nom = noms[j];
  		sabot[j].valeur = j+1; /*pour l'as, la valeur par défaut est 1 mais peut devenir 10 selon le choix du joueur*/
		sabot[j].nbCartes = 6*4;
 	 }
	
	for (j = 10; j < 13; j++) {
		sabot[j].nom = noms[j];
		sabot[j].valeur = 10;
		sabot[j].nbCartes = 6*4;
	}
	printf("Quelle est la première carte que vous avez reçue?");
	char carte1[20];
	scanf("%s", carte1); 
	printf("Quelle est la deuxième carte que vous avez reçue?");
	char carte2[20];
	scanf("%s", carte2);
	
	printf("Quelle est la carte du croupier?");
	char cartec[20];
	scanf("%s", cartec); 
	
	
	/*printf("la carte que vous avez choisi est: %c\n" , sabot[11].nom);*/
	return 0;
}
	
	
 
