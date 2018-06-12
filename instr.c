#include "instr.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 


char tab_instr[1024][4][16];


int indice_instr = 0 ;


int indice_instr_courant(){ return indice_instr; }


void ajouter_instr(char *op, char *a, char *b, char *c){

	strcpy(tab_instr[indice_instr][0], op);
	strcpy(tab_instr[indice_instr][1], a);
	strcpy(tab_instr[indice_instr][2], b);
	strcpy(tab_instr[indice_instr][3], c);
	indice_instr++;

}


void patch_instr(int i){ sprintf(tab_instr[i][1], "%d", indice_instr); }


void afficher_instr(){

	printf("\n--- TABLE DES INSTRUCTIONS ---\n");
int i ;
	for(i = 0 ; i<indice_instr ; i++){
		printf("%3d\t %s\t %s\t %s\t %s\n", i, tab_instr[i][0], tab_instr[i][1], tab_instr[i][2], tab_instr[i][3]);
	}
	printf("\n");

}

int mem[1024];
int reg[16];

void execute_instr(void) {

	FILE *f = fopen("asm.hex", "w");

	int ip = 0;
	while (ip != (indice_instr - 1)) {
		if (strcmp(tab_instr[ip][0], "ADD") == 0) {
			reg[atoi(&tab_instr[ip][1][1])] = reg[atoi(&tab_instr[ip][2][1])] + reg[atoi(&tab_instr[ip][3][1])];
			fprintf(f, "%02d%02d00%02d\n", atoi(&tab_instr[ip][3][1]), atoi(&tab_instr[ip][2][1]), atoi(&tab_instr[ip][1][1]));
			ip++;
		}
		if (strcmp(tab_instr[ip][0], "SOU") == 0) {
			reg[atoi(&tab_instr[ip][1][1])] = reg[atoi(&tab_instr[ip][2][1])] - reg[atoi(&tab_instr[ip][3][1])];
			fprintf(f, "%02d%02d01%02d\n", atoi(&tab_instr[ip][3][1]), atoi(&tab_instr[ip][2][1]), atoi(&tab_instr[ip][1][1]));
			ip++;
		}
		if (strcmp(tab_instr[ip][0], "DIV") == 0) {
			reg[atoi(&tab_instr[ip][1][1])] = reg[atoi(&tab_instr[ip][2][1])] / reg[atoi(&tab_instr[ip][3][1])];
			fprintf(f, "%02d%02d0C%02d\n", atoi(&tab_instr[ip][3][1]), atoi(&tab_instr[ip][2][1]), atoi(&tab_instr[ip][1][1]));
			ip++;
		}
		if (strcmp(tab_instr[ip][0], "MUL") == 0) {
			reg[atoi(&tab_instr[ip][1][1])] = reg[atoi(&tab_instr[ip][2][1])] * reg[atoi(&tab_instr[ip][3][1])];
			fprintf(f, "%02d%02d02%02d\n", atoi(&tab_instr[ip][3][1]), atoi(&tab_instr[ip][2][1]), atoi(&tab_instr[ip][1][1]));
			ip++;
		}
		if (strcmp(tab_instr[ip][0], "AFC") == 0) {
			reg[atoi(&tab_instr[ip][1][1])] = atoi(tab_instr[ip][2]);
			fprintf(f, "00%02d0A%02d\n", atoi(tab_instr[ip][2]), atoi(&tab_instr[ip][1][1]));
			ip++;
		}
		if (strcmp(tab_instr[ip][0], "STORE") == 0) {
			mem[atoi(tab_instr[ip][1])] = reg[atoi(&tab_instr[ip][2][1])];
			fprintf(f, "00%02d0D%02d\n", atoi(&tab_instr[ip][2][1]), atoi(tab_instr[ip][1]));
			ip++;
		}
		if (strcmp(tab_instr[ip][0], "LOAD") == 0) {
			reg[atoi(&tab_instr[ip][1][1])] = mem[atoi(tab_instr[ip][2])];
			fprintf(f, "00%02d0E%02d\n", atoi(tab_instr[ip][2]), atoi(&tab_instr[ip][1][1]));
			ip++;
		}
		if (strcmp(tab_instr[ip][0], "JMP") == 0) {
			ip = atoi(tab_instr[ip][1]) ;
		}
		if (strcmp(tab_instr[ip][0], "JMPC") == 0) {
			if(reg[atoi(&tab_instr[ip][2][1])] == 1){ ip ++;} // 0 ou 1 ?
			else {ip = atoi(tab_instr[ip][1]);} 
		}

		if (strcmp(tab_instr[ip][0], "SUP") == 0) {
			if(reg[atoi(&tab_instr[ip][2][1])] > reg[atoi(&tab_instr[ip][3][1])])
			{ reg[atoi(&tab_instr[ip][1][1])] = 1 ;}
			else {reg[atoi(&tab_instr[ip][1][1])] = 0 ;}
			fprintf(f, "%02d%02d06%02d\n", atoi(&tab_instr[ip][3][1]), atoi(&tab_instr[ip][2][1]), atoi(&tab_instr[ip][1][1]));
			ip++;
		}
		if (strcmp(tab_instr[ip][0], "SUPE") == 0) {
			if(reg[atoi(&tab_instr[ip][2][1])] >= reg[atoi(&tab_instr[ip][3][1])])
			{ reg[atoi(&tab_instr[ip][1][1])] = 1 ;}
			else {reg[atoi(&tab_instr[ip][1][1])] = 0 ;}
			fprintf(f, "%02d%02d08%02d\n", atoi(&tab_instr[ip][3][1]), atoi(&tab_instr[ip][2][1]), atoi(&tab_instr[ip][1][1]));
			ip++;
		}

		if (strcmp(tab_instr[ip][0], "INF") == 0) {
			if(reg[atoi(&tab_instr[ip][2][1])] < reg[atoi(&tab_instr[ip][3][1])])
			{ reg[atoi(&tab_instr[ip][1][1])] = 1 ;}
			else {reg[atoi(&tab_instr[ip][1][1])] = 0 ;}
			fprintf(f, "%02d%02d05%02d\n", atoi(&tab_instr[ip][3][1]), atoi(&tab_instr[ip][2][1]), atoi(&tab_instr[ip][1][1]));
			ip++;
		}
		if (strcmp(tab_instr[ip][0], "INFE") == 0) {
			if(reg[atoi(&tab_instr[ip][2][1])] <= reg[atoi(&tab_instr[ip][3][1])])
			{ reg[atoi(&tab_instr[ip][1][1])] = 1 ;}
			else {reg[atoi(&tab_instr[ip][1][1])] = 0 ;}
			fprintf(f, "%02d%02d07%02d\n", atoi(&tab_instr[ip][3][1]), atoi(&tab_instr[ip][2][1]), atoi(&tab_instr[ip][1][1]));
			ip++;
		}

		if (strcmp(tab_instr[ip][0], "AND") == 0) {
			if(reg[atoi(&tab_instr[ip][2][1])] == 1 && reg[atoi(&tab_instr[ip][3][1])] == 1)
			{ reg[atoi(&tab_instr[ip][1][1])] = 1 ;}
			else {reg[atoi(&tab_instr[ip][1][1])] = 0 ;}
			fprintf(f, "%02d%02d0F%02d\n", atoi(&tab_instr[ip][3][1]), atoi(&tab_instr[ip][2][1]), atoi(&tab_instr[ip][1][1]));
			ip++;
		}
		if (strcmp(tab_instr[ip][0], "OR") == 0) {
			if(reg[atoi(&tab_instr[ip][2][1])] == 1 || reg[atoi(&tab_instr[ip][3][1])] == 1)
			{ reg[atoi(&tab_instr[ip][1][1])] = 1 ;}
			else {reg[atoi(&tab_instr[ip][1][1])] = 0 ;}
			fprintf(f, "%02d%02d10%02d\n", atoi(&tab_instr[ip][3][1]), atoi(&tab_instr[ip][2][1]), atoi(&tab_instr[ip][1][1]));
			ip++;
		}

		if (strcmp(tab_instr[ip][0], "EQU") == 0) {
			if(reg[atoi(&tab_instr[ip][2][1])] == reg[atoi(&tab_instr[ip][3][1])])
			{ reg[atoi(&tab_instr[ip][1][1])] = 1 ;}
			else {reg[atoi(&tab_instr[ip][1][1])] = 0 ;}
			fprintf(f, "%02d%02d09%02d\n", atoi(&tab_instr[ip][3][1]), atoi(&tab_instr[ip][2][1]), atoi(&tab_instr[ip][1][1]));
			ip++;
		}
		if (strcmp(tab_instr[ip][0], "NEQ") == 0) {
			if(reg[atoi(&tab_instr[ip][2][1])] != reg[atoi(&tab_instr[ip][3][1])])
			{ reg[atoi(&tab_instr[ip][1][1])] = 1 ;}
			else {reg[atoi(&tab_instr[ip][1][1])] = 0 ;}
			fprintf(f, "%02d%02d11%02d\n", atoi(&tab_instr[ip][3][1]), atoi(&tab_instr[ip][2][1]), atoi(&tab_instr[ip][1][1]));
			ip++;
		}
		if (strcmp(tab_instr[ip][0], "NO") == 0) {
			if(reg[atoi(&tab_instr[ip][2][1])] == 0){ reg[atoi(&tab_instr[ip][1][1])] = 1;}
			else { reg[atoi(&tab_instr[ip][1][1])] = 0 ;}
			fprintf(f, "%02d%02d12%02d\n", atoi(&tab_instr[ip][3][1]), atoi(&tab_instr[ip][2][1]), atoi(&tab_instr[ip][1][1]));
			ip++;
		}
		
	} 
	fclose(f);

}
