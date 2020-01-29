#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "frontend.h"
#include "ast.h"
#include "semantic.h"
#include "calculator.tab.h"

const char * get_type_str(int vartype) {
  switch (vartype) {
    case TYPEINTEGER:
      return "TYPEINTEGER";
    case TYPEFLOAT:
      return "TYPEFLOAT";
		default:
			yyerror("Tipo indefinido\n");
			abort();
  }
}

static unsigned symhash(char *sym)
{
	unsigned int hash = 0;
	unsigned c;

	while((c = *sym++)) hash = hash*9 ^ c;

	return hash;
}

struct symbol * getcreate_symbol(char *sym) {
	struct symbol *sp = &symtab[symhash(sym)%NHASH];
	int scount = NHASH;		

	while(--scount >= 0) {
    if(sp->name && !strcmp(sp->name, sym)) { return sp; }

		if(!sp->name) {	
			sp->name = strdup(sym);
			sp->type = -1;
			sp->value = 0;
			return sp;
		}

    if(++sp >= symtab+NHASH) sp = symtab; 
	}
	yyerror("Tabela de simbolos overflow\n");
	abort(); 
}

struct symbol * get_symbol(char *sym) {
	struct symbol *sp = &symtab[symhash(sym)%NHASH];
	int scount = NHASH;	
	while(--scount >= 0) {
    if(sp->name && !strcmp(sp->name, sym)) { return sp; }

		if(!sp->name) {	
			return NULL;
		}
    
		if(++sp >= symtab+NHASH) sp = symtab; 
	}
	yyerror("Erro na criação de símbolos\n");
	abort(); 
}

void valid_types(struct evalexp * l, struct evalexp * r) {
	if (l->type == r->type) {
		return;
	}
	char str[150];
	snprintf(
		str, 
		sizeof str, 
		"ERRO semantico, não é possível realizar operações com tipos diferentes l:%s r:%s",
		get_type_str(l->type),
		get_type_str(r->type)
	);
	yyerror(str);
	exit(1);
}

struct evalexp * eval_tree(struct ast *a) {
    struct symbol *symbol;
	struct symbol *new_symbol;
	struct evalexp *r;
	struct evalexp *l;

	if(!a) {
		yyerror("Internal error, NULL evalexp");
		return NULL;
	}

	struct evalexp * ee = malloc(sizeof(struct evalexp));

	switch (a->nodetype) {
		case T_INTEGER:
			ee->type = TYPEINTEGER;
			ee->value = (int)((struct numvar *)a)->number;
			break;
		case T_FLOAT:
			ee->type = TYPEFLOAT;
			ee->value = (double)((struct numvar *)a)->number;
			break;
		case T_VAR:
			symbol = get_symbol(((struct var *)a)->name);
			if (symbol) {
				ee->value = symbol->value;
				ee->type = symbol->type;
			} else {
				yyerror("ERRO semantico, variavel não declarada");
				exit(1);
			}
			break;

		case T_ADD:
			l = eval_tree(a->l);
			r = eval_tree(a->r);
			valid_types(l, r);
			ee->type = l->type;
			ee->value = l->value + r->value;
			break;
		case T_SUB:
			l = eval_tree(a->l);
			r = eval_tree(a->r);
			valid_types(l, r);
			ee->type = l->type;
			ee->value = l->value - r->value;
			break;
		case T_MUL:
			l = eval_tree(a->l);
			r = eval_tree(a->r);
			valid_types(l, r);
			ee->type = l->type;
			ee->value = l->value * r->value;
			break;
		case T_DIV:
			l = eval_tree(a->l);
			r = eval_tree(a->r);
			valid_types(l, r);
			ee->type = l->type;
			ee->value = l->value / r->value;
			break;
		case T_EXP:
			l = eval_tree(a->l);
			r = eval_tree(a->r);
			valid_types(l, r);
			ee->type = TYPEFLOAT;
			ee->value = pow(l->value,r->value);
			break;
		case T_MINUS:
			l = eval_tree(a->l);
			ee->type = l->type;
			ee->value = -l->value;
			break;

		case T_ASSIGNMENT:
			new_symbol = getcreate_symbol(((struct var *)a->l)->name);
			r = eval_tree(a->r);
			if ((new_symbol->type != -1) && (new_symbol->type != r->type)) {
				char str[150];
				snprintf(
					str, 
					sizeof str, 
					"ERRO semantico, variavel %s tem tipo %s e não pode receber um tipo %s", 
					new_symbol->name, 
					get_type_str(new_symbol->type), 
					get_type_str(r->type)
				);
				yyerror(str);
				exit(1);
			}

			new_symbol->value = r->value;
			new_symbol->type = r->type;
			ee->value = r->value;
			ee->type = r->type;
			break;
		case T_EXPRESSION:
			l = eval_tree(a->l);
			ee = l;
			break;
		case T_PRINT:
			l = eval_tree(a->l);
			ee = l;
			// Colocando ; para printf não corromper o código llvm dump 
			// if (ee->type == TYPEINTEGER) printf("; %d\n", (int)ee->value);
			// else printf("; %f\n", ee->value);
			break;
	}
	yylloc.first_column++;
	return ee;
}