#include <stdio.h>
#include <stdlib.h>
#include "ll.h"
#include "ast.h"

llnode create_node() {
    llnode temp; 
    temp = (llnode)malloc(sizeof(struct ll));
    temp->next = NULL;
    return temp;
}

llnode add_node(llnode head, struct ast *tree) {
    llnode temp, p;
    temp = create_node();
    temp->tree = tree;
    if (head == NULL) {
        head = temp;
    } else {
        p = head;
        // ** Implementação ruim
        while(p->next != NULL) {
            p = p->next;
        }
        p->next = temp;
    }
    return head;
}