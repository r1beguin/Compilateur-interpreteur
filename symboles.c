#include "symboles.h"
#include <stdio.h>
#include <string.h>


struct symbole
{
	char id[16] ;
	char type[16];
};


struct symbole tableau_symboles[256];


int indice_symb = 0 ;


struct symbole creer_symbole(char *identifiant, char *type_symbole){

	struct symbole Symbole ;
	strcpy(Symbole.id, identifiant);
	strcpy(Symbole.type, type_symbole);

	return Symbole;

}


void ajouter_symbole(char *identifiant, char *type_symbole){

	struct symbole Symbole = creer_symbole(identifiant, type_symbole);
	tableau_symboles[indice_symb] = Symbole;
	indice_symb++;

}


int indice_courant(){ return indice_symb; }


int chercher_symbole(char *ident){
	int i;
	for(i = 0 ; i <= 256 ; i++){
		if(strcmp(tableau_symboles[i].id, ident) == 0){ return i ; }
	}

}


void supprimer_symbole(){ indice_symb--; }


void afficher_symboles(){

	printf("\n--- TABLE DES SYMBOLES ---\n");
	int i ;
	for(i = 0 ; i<indice_symb ; i++){
		printf("%d %s\t %s\n", i, tableau_symboles[i].type, tableau_symboles[i].id);
	}
	printf("\n");

}





