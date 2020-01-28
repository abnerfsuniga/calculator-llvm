#ifndef LL_H
#define LL_H

#include <stdio.h>

struct ll {
    struct ast *tree;
    struct ll *next;
};

typedef struct ll * llnode;

struct ll * tree_list;

llnode create_node();
llnode add_node();

#endif