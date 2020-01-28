#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "frontend.h"

struct ast *newast(int nodetype, struct ast *l, struct ast *r) {
  struct ast *a = malloc(sizeof(struct ast));
  if(!a) {
    yyerror("Sem espaço");
    exit(0);
  }
  a->nodetype = nodetype;
  a->l = l;
  a->r = r;
  return a;
}

struct ast *newinteger(double d) {
  struct numvar *a = malloc(sizeof(struct numvar));
  if(!a) {
    yyerror("Sem espaço");
    exit(0);
  }
  a->nodetype = T_INTEGER;
  a->number = d;
  return (struct ast *)a;
}

struct ast *newfloat(double d) {
  struct numvar *a = malloc(sizeof(struct numvar));
  if(!a) {
    yyerror("Sem espaço");
    exit(0);
  }
  a->nodetype = T_FLOAT;
  a->number = d;
  return (struct ast *)a;
}


struct ast *newvar(char *name) {
	struct var *a = malloc(sizeof(struct var));
	if(!a) {
    yyerror("Sem espaço");
    exit(0);
  }
	a->nodetype = T_VAR;
	a->name = name;
	return (struct ast *)a;
}

void printtree(struct ast *a, int level, FILE *file) {
	fprintf(file, ";%*s", 8*level, "");
	level++;

	if(!a) {
    fprintf(file, "NULL\n");
    return;
  }

	switch(a->nodetype) {
		case T_INTEGER:
      fprintf(file, "%d\n", (int)((struct numvar *)a)->number);
			return;
    case T_FLOAT:
			fprintf(file, "%.2f\n", ((struct numvar *)a)->number);
			return;
		case T_VAR:
			fprintf(file, "%s\n", ((struct var *)a)->name);
			return;

		case T_ADD:
		case T_SUB:
		case T_MUL:
		case T_DIV:
		case T_EXP:
    case T_ASSIGNMENT:
      fprintf(file, "%s\n", get_node_name(a->nodetype));
			printtree(a->l, level, file);
			printtree(a->r, level, file);
			return;

		case T_MINUS:
    case T_PRINT:
    case T_EXPRESSION:
		  fprintf(file, "%s\n", get_node_name(a->nodetype));
			printtree(a->l, level, file);
			return;

	  default: 
			fprintf(file, "BAD NODE %c\n", a->nodetype);
    	return;
	}
}

const char * get_node_name(int nodetype) {
  switch (nodetype) {
    case T_ASSIGNMENT:
      return "T_ASSIGNMENT";
    case T_EXPRESSION:
      return "T_EXPRESSION";
    case T_PRINT:
      return "T_PRINT";
    case T_ADD:
      return "+";
    case T_SUB:
      return "-";
    case T_MUL:
      return "*";
    case T_DIV:
      return "/";
    case T_EXP:
      return "^";
    case T_MINUS:
      return "M";
    case T_INTEGER:
      return "INT";
    case T_FLOAT:
      return "FLOAT";
    case T_VAR:
      return "VAR";
    default:
			yyerror("Nó indefinido\n");
			abort();
  }
}