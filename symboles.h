#include <stdio.h>

struct symbole creer_symbole(char *identifiant, char *type_symbole);

void ajouter_symbole(char *identifiant, char *type_symbole);

int indice_courant(void);

int chercher_symbole(char *id);

void supprimer_symbole(void);

void afficher_symboles(void);

