#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*
 * XXX: would this be faster if we do not use recursion BUT DO I CARE?? nope.
 */

#include "token_avl_tree.h"

token_avl_tree_node_t *token_avl_tree_node_gen(token_t root, int indx)
{
	token_avl_tree_node_t *v = calloc(1, sizeof(token_avl_tree_node_t));
	v->value = root;
	v->index = indx;

	return v;
}

void token_avl_tree_node_delete_ungen(token_avl_tree_node_t *v)
{
	if(v == NULL) return;

	token_avl_tree_node_delete_ungen(v->left);
	token_avl_tree_node_delete_ungen(v->right);

	free(v);
}

static int fixup_height(token_avl_tree_node_t *root)
{
	if(!root)
		return 0;
	if(root->right == 0 && root->left == 0)
	{
		return (root->height = 0);
	}

	const int a = fixup_height(root->left);
	const int b = fixup_height(root->right);

	return (root->height = (a > b ? a : b) + 1);
}

static int insert(token_avl_tree_node_t **root, token_avl_tree_node_t *sib) 
{
	// Inserting Node

	int k = 0;

	if(token_equ(root[0]->value, sib->value)) return -1;
	else if(token_less(root[0]->value, sib->value))
	{
		if(root[0]->right == NULL)
			root[0]->right = sib;
		else
			k = insert(&(root[0]->right), sib);

		root[0]->height += !root[0]->left;
	}
	else
	{
		if(root[0]->left == NULL)
			root[0]->left = sib;
		else
			k = insert(&(root[0]->left), sib);

		root[0]->height += !root[0]->right;
	}

	// Balancing the Node
	int a = root[0]->right ? root[0]->right->height : 0;
	int b = root[0]->left ? root[0]->left->height : 0;

	if(a - b > 1)
	{
		token_avl_tree_node_t *nrl = root[0]->right->left;
		token_avl_tree_node_t *xxx = root[0];
		root[0] = root[0]->right;
		root[0]->left = xxx;
		root[0]->left->right = nrl;
	}
	else if(a - b < -1)
	{
		token_avl_tree_node_t *nlr = root[0]->left->right;
		token_avl_tree_node_t *xxx = root[0];
		root[0] = root[0]->left;
		root[0]->right = xxx;
		root[0]->right->left = nlr;
	}

	fixup_height(root[0]);

	return k;
}

int token_avl_tree_find(token_avl_tree_node_t *v, token_t t)
{
	if(!v) return -1;

	// printf("%d %d %s | ", v->index, token_less(v->value, t), token_to_str(v->value));

	if(token_equ(t, v->value))
		return (int)v->index;

	if(token_less(t, v->value))
		return token_avl_tree_find(v->left, t);
	else
		return token_avl_tree_find(v->right, t);
}

int token_avl_tree_node_insert(token_avl_tree_node_t **t, token_t v, int indx)
{
	token_avl_tree_node_t *n = calloc(1, sizeof(token_avl_tree_node_t));
	n->value = v;
	n->index = indx;
	n->height = 0;

	return insert(t, n);
}

int token_less(token_t t1, token_t t2)
{
	const uint32_t ut1 = *(uint32_t *)(void *)(token_t *)&t1;
	const uint32_t ut2 = *(uint32_t *)(void *)(token_t *)&t2;
	
	return ut1 < ut2 ;
}

int token_equ(token_t t1, token_t t2)
{
	const uint32_t ut1 = *(uint32_t *)(void *)(token_t *)&t1;
	const uint32_t ut2 = *(uint32_t *)(void *)(token_t *)&t2;
	
	return ut1 == ut2 ;
}

void token_avl_tree_print(token_avl_tree_node_t *t, int depth)
{
	if(t == NULL)
		return;

	printf("%*c%s : %d %23d\n", 4 * depth, ' ', token_to_str(t->value), t->height, t->index);

	token_avl_tree_print(t->left, depth + 1);
	token_avl_tree_print(t->right, depth + 1);
}

void token_avl_tree_iterate(token_avl_tree_node_t *t, void(*foreach)(token_t key, int value))
{
	if(t == NULL) return;

	foreach(t->value, t->index);

	token_avl_tree_iterate(t->left, foreach);
	token_avl_tree_iterate(t->right, foreach);
}
