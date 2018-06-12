all : compiler
lex.yy.c : compil.lex
	./flex compil.lex
rule.tab.h rule.tab.c : rule.y symboles.h instr.h
	~/bison/bin/bison -d -v rule.y
compiler : lex.yy.c rule.tab.c rule.tab.h symboles.c instr.c
	gcc -g -o compiler lex.yy.c rule.tab.c symboles.c instr.c libfl.a ~/bison/lib/liby.a
test : compiler
	./compiler < test.c
