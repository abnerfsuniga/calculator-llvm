%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "frontend.h"
#include "ll.h"
%}
%locations
%union {
  struct ast *a;
	char *name;
  double d;
}

%type <a> exp factor exponential term statement
%token <d> INTEGER FLOAT
%token <name> VARIABLE

%token ADD SUB MUL DIV EXP
%token EOL EQUAL PRINT FINAL
%left LEFT
%right RIGHT

%start calculation

%%
calculation: list           {  } 

list: statement             { tree_list = add_node(tree_list, $1); }
| list statement            { tree_list = add_node(tree_list, $2); }

statement: 
	VARIABLE EQUAL exp FINAL  { $$ = newast(T_ASSIGNMENT, newvar($1), $3); }
| PRINT exp FINAL         { $$ = newast(T_PRINT, $2, NULL); }
;


exp: factor                 
| exp ADD factor          	{ $$ = newast(T_ADD, $1, $3); }
| exp SUB factor          	{ $$ = newast(T_SUB, $1, $3); }
;

factor: exponential         
| factor MUL exponential    { $$ = newast(T_MUL, $1, $3); }
| factor DIV exponential    { $$ = newast(T_DIV, $1, $3); }
;

exponential: term           
| exponential EXP term      { $$ = newast(T_EXP, $1, $3); }
;

term: FLOAT                 { $$ = newfloat($1); }
| INTEGER								    { $$ = newinteger($1); }
| VARIABLE                  { $$ = newvar($1); }
| LEFT exp RIGHT            { $$ = $2; }
| SUB term                  { $$ = newast(T_MINUS, $2, NULL); }
;

%%