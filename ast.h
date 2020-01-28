#include <stdio.h>

struct ast *AST;

enum nodetype {
  T_ROOT,
  T_STATEMENT,
  T_ASSIGNMENT,
  T_EXPRESSION,
  T_PRINT,
  T_ADD,
  T_SUB,
  T_MUL,
  T_DIV,
  T_EXP,
  T_MINUS,
  T_INTEGER,
  T_FLOAT,
  T_VAR
};

const char * get_node_name();

struct ast {
  int nodetype;
  struct ast *l;
  struct ast *r;
};

struct numvar {
  int nodetype;
  double number;
};

struct var {
	int nodetype;
	char *name;
};

struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newinteger(double d);
struct ast *newfloat(double d);
struct ast *newvar(char *name);

void printtree(struct ast *a, int level, FILE *file);
