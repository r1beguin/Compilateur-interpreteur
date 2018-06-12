%{
#include <stdio.h>
#include "symboles.h"
#include "instr.h"
int yylex(void);
void yyerror(char*);

#define binop(op)\
sprintf(str, "%d", indice_courant()-2) ;\
ajouter_instr("LOAD", "R1", str, "");\
sprintf(str, "%d", indice_courant()-1) ;\
ajouter_instr("LOAD", "R2", str, "");\
ajouter_instr(op, "R1", "R1", "R2");\
sprintf(str, "%d", indice_courant()-2) ;\
ajouter_instr("STORE", str, "R1", "");\
supprimer_symbole(); supprimer_symbole();\
ajouter_symbole("#", "TEMP " op);

char str[16] ; 

%}

%union {
	int nb;
	char id[16];
}

%token tMAIN tPRINTF tIF tELSE tWHILE tFOR tRETURN
%token tAO tAF tPO tPF 
%token tCONST tINT tVARIABLE tNOMBRE tVOID
%token tADD tSOUS tMULT tDIV tEQUAL 
%token tAND tOR tEQ tSUP tSUPEQ tINF tINFEQ tNO tNOEQ
%token tVIRG tFIN_INST 

%type <nb> tNOMBRE if_act if_jmp tWHILE
%type <id> tVARIABLE

%left tAND tOR
%left tSUP tINF tSUPEQ tINFEQ tNOEQ tEQ
%left tADD tSOUS
%left tMULT tDIV
%left tNO

%nonassoc tTRUC
%nonassoc tELSE

%% 

start : tINT tMAIN tPO tPF body {afficher_symboles(); afficher_instr();} ;
body : tAO instructions tAF ;

instruction : declaration tFIN_INST
	| constante tFIN_INST
	| affichage tFIN_INST
	| operation tFIN_INST 
	| if 
	| while ;

instructions : instruction instructions
	| ; 


if : tIF tPO operations tPF if_act body_if %prec tTRUC {patch_instr($5);} 
	| tIF tPO operations tPF if_act body_if tELSE if_jmp {patch_instr($5);} body_if {patch_instr($8);};

if_act : {$$ = indice_instr_courant(); ajouter_instr("JMPC", "-1", "R1", "");} ;
if_jmp : {$$ = indice_instr_courant(); ajouter_instr("JMP", "-2", "", "");} ;

body_if : instruction
	| body ;

while : tWHILE tPO {$1 = indice_instr_courant();} operations tPF if_act body 
{sprintf(str, "%d", $1); ajouter_instr("JMP", str, "", ""); patch_instr($6);};

declaration : tINT tVARIABLE {ajouter_symbole($2, "int");} declarations ;
declarations : tVIRG tVARIABLE {ajouter_symbole($2, "int");} declarations
	| ;

constante : tCONST tINT tVARIABLE tEQUAL tNOMBRE {ajouter_symbole($3, "const");
												  sprintf(str, "%d", $5);
												  ajouter_instr("AFC", "R1", str, "");
												  sprintf(str, "%d", indice_courant());
			   								      ajouter_instr("STORE", str, "R1", "");
												  ajouter_symbole("#", "TEMP NB CONST"); 
												  sprintf(str, "%d", indice_courant()-1) ;
												  ajouter_instr("LOAD", "R1", str, "");
	   										      sprintf(str, "%d", chercher_symbole($3));
										 		  ajouter_instr("STORE", str, "R1", "");
												  supprimer_symbole();};

affichage : tPRINTF tPO tVARIABLE tPF {printf("Affichage %s\n", $3);} ;

operation : tVARIABLE tEQUAL operations {sprintf(str, "%d", indice_courant()-1) ; 											 ajouter_instr("LOAD", "R1", str, "");
										 sprintf(str, "%d", chercher_symbole($1));
										 ajouter_instr("STORE", str, "R1", "");
										 supprimer_symbole();} ;

operations : tPO operations tPF
	| operations tADD operations {binop("ADD");}
	| operations tSOUS operations {binop("SOU");}
	| operations tDIV operations {binop("DIV");}
	| operations tMULT operations {binop("MUL");}
	| tVARIABLE {sprintf(str, "%d", chercher_symbole($1));
				 ajouter_instr("LOAD", "R1", str, "") ;
				 sprintf(str, "%d", indice_courant());
				 ajouter_instr("STORE", str, "R1", "");
				 ajouter_symbole("#", "TEMP VARIABLE");}
	| tNOMBRE {sprintf(str, "%d", $1);
			   ajouter_instr("AFC", "R1", str, "");
			   sprintf(str, "%d", indice_courant());
			   ajouter_instr("STORE", str, "R1", "");
			   ajouter_symbole("#", "TEMP NB"); } 
	| operations tAND operations {binop("AND");}
	| operations tOR operations {binop("OR");}
	| operations tSUP operations {binop("SUP");}
	| operations tSUPEQ operations {binop("SUPE");}
	| operations tINF operations {binop("INF");}
	| operations tINFEQ operations {binop("INFE");}
	| operations tNOEQ operations {binop("NEQ");}
	| operations tEQ operations {binop("EQU");}
	| tNO operations {sprintf(str, "%d", indice_courant()-1) ;
					  ajouter_instr("LOAD", "R1", str, "");
					  ajouter_instr("NO", "R1", "R1", "");
					  sprintf(str, "%d", indice_courant()-1) ;
					  ajouter_instr("STORE", str, "R1", "");
					  supprimer_symbole();
					  ajouter_symbole("#", "TEMP NO");} ;

%%

int main(){
	yyparse();
	execute_instr();
}

