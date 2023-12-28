#ifndef _TRANSITION_H_
#define _TRANSITION_H_

// Basically a wrapper over the avl tree, making things look nice and usable

#include "token_avl_tree.h"

typedef struct
{
	token_avl_tree_node_t *token_state_map;
} transitions_t;

// NOTE: expcted zero init

int transition_emplace (transitions_t *a, token_t b, int c);
int  transition_fetch   (transitions_t *a, token_t b);
void transition_delete  (transitions_t *c);

#endif
