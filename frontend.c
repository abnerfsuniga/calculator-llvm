#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include "ast.h"
#include "frontend.h"
#include "ll.h"
#include "calculator.tab.h"
#include "semantic.h"

llnode frontend(char* filename) { 
	FILE *code_file = fopen(filename, "r");

	if (!code_file) {
		printf("Não foi possível abrir arquivo contendo código\n");
		exit(1);
	}

	// Análise léxica e sintática
	yyin = code_file;
	yyparse();

	llnode p;
	p = tree_list;

	// Análise semântica
	yylloc.first_line = 1; 
	while(p != NULL){
		yylloc.first_column = 1;
		struct evalexp *ee = eval_tree(p->tree);
		yylloc.first_line++;
		p = p->next;
	}

	p = tree_list;
	return p;
}

void yyerror(char *s, ...) {
	va_list ap;
	va_start(ap, s);
	fprintf(stderr, "\n");
	fprintf(stderr, "ERRO na linha %d: coluna: %d.\n%s\n", yylloc.first_line, yylloc.first_column, s);
	exit(0);
}