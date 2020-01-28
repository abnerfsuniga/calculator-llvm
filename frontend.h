#include <stdio.h>
#include "ll.h"

extern int yylineno;
extern FILE *yyin;
extern int yylex();
extern int yyparse();

llnode frontend(char* filename);

void yyerror(char *s, ...);
