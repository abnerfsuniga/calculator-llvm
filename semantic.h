#include <stdio.h>
#include <stdlib.h>
#include <llvm-c/Types.h>

enum vartype {
	TYPEINTEGER,
	TYPEFLOAT
};

struct symbol {
	char *name;
	int type;
	double value;
	LLVMValueRef llvm_value;
};

#define NHASH 9997
struct symbol symtab[NHASH];

struct evalexp {
	double value;
	int type;
};

struct evalexp * eval_tree(struct ast *a);
struct symbol * getcreate_symbol(char *sym);
struct symbol * get_symbol(char *sym);
const char * get_type_str(int vartype);

