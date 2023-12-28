#ifndef _TOKEN_AVL_TREE_H_
#define _TOKEN_AVL_TREE_H_

#include <stdint.h>
#include "expression.h"

typedef struct token_avl_tree_node_t
{
	token_t value;
	struct token_avl_tree_node_t *left;
	struct token_avl_tree_node_t *right;
	int32_t index;
	int32_t height;
} token_avl_tree_node_t;

token_avl_tree_node_t *token_avl_tree_node_gen(token_t root, int indx);
void token_avl_tree_node_delete_ungen(token_avl_tree_node_t *);

int token_avl_tree_node_insert(token_avl_tree_node_t **, token_t, int);

int token_avl_tree_find(token_avl_tree_node_t *, token_t);

int token_less(token_t t1, token_t t2);
int token_equ(token_t t1, token_t t2);

void token_avl_tree_print(token_avl_tree_node_t *t, int depth);

void token_avl_tree_iterate(token_avl_tree_node_t *t, void(*foreach)(token_t key, int value));

#endif
