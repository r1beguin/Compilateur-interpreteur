%{
#include "rule.tab.h"
%}

NB -?[0-9]+(E[+\-][0-9]+)? 

NOM [a-zA-Z]([a-zA-Z]|{NB}|\_)*

%%

main return(tMAIN);
printf return(tPRINTF);
if return(tIF);
else return(tELSE);
while return(tWHILE);

\{ return(tAO);
\} return(tAF);
\( return(tPO);
\) return(tPF);

{NB} { yylval.nb = (int) atof(yytext); return(tNOMBRE); }
const return(tCONST);
int return(tINT);
{NOM} { strcpy(yylval.id, yytext); return(tVARIABLE); }

\+ return(tADD);
\- return(tSOUS);
\* return(tMULT);
\/ return(tDIV);
\= return(tEQUAL);

\=\= return(tEQ);
\> return(tSUP);
\>\= return(tSUPEQ);
\< return(tINF);
\<\= return(tINFEQ);
\! return(tNO);
\!\= return(tNOEQ);

&& return(tAND);
\|\| return(tOR);

\, return(tVIRG);
\; return(tFIN_INST);

[ \t\n]	;
%%

