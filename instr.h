#include <stdio.h>

int indice_instr_courant(void);

void ajouter_instr(char *op, char *a, char *b, char *c);

void patch_instr(int i);

void afficher_instr(void);

void execute_instr(void);
