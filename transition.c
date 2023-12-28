#include "transition.h"

int transition_emplace(transitions_t *t, token_t a, int i)
{
	if(t->token_state_map == NULL)
		t->token_state_map = token_avl_tree_node_gen(a, i);
	else
		return token_avl_tree_node_insert(&t->token_state_map, a, i);
	return 0;
}

int  transition_fetch(transitions_t *t, token_t a)
{
	return token_avl_tree_find(t->token_state_map, a);
}

void transition_delete(transitions_t *t)
{
	token_avl_tree_node_delete_ungen(t->token_state_map);
}
