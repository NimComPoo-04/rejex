#include <stdio.h>

#include "expression.h"
#include "transition.h"
#include "state.h"
#include "capture.h"

char expr[] = " ?.+ ";

char value[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit,"
"sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim"
"ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex"
"ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit"
"esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat"
"non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

void cap(int a, int b)
{
	printf("%d %d %.*s\n", a, b, b - a, &value[a]);
}

int main(void)
{
	expression_t e = {.expr_str = expr, .expr_str_size = sizeof expr - 1};

	if(expression_tokenize(&e))
	{
		fprintf(stderr, "FATAL ERROR!\n");
		return 1;
	}
	expression_print(&e);

	/*
	token_avl_tree_node_t *t = token_avl_tree_node_gen(e.token_vector[0], 0);

	for(size_t i = 1; i < e.token_vector_size; i++)
	{
		token_avl_tree_node_insert(&t, e.token_vector[i], i);
	}

	for(size_t i = 0; i < e.token_vector_size; i++)
	{
		if(i == (size_t)token_avl_tree_find(t, e.token_vector[i]))
			printf("PASSING %ld!\n", i);
		else
			printf("FAILING %ld!\n", i);
	}

	token_avl_tree_print(t, 0);
	*/

	/*
	transitions_t t = {0};

	for(size_t i = 0; i < e.token_vector_size; i++)
		transition_emplace(&t, e.token_vector[i], i);

	token_avl_tree_print(t.token_state_map, 0);

	for(size_t i = 0; i < e.token_vector_size; i++)
	{
		if(i == (size_t)transition_fetch(&t, e.token_vector[i]))
			printf("PASSING %ld!\n", i);
		else
			printf("FAILING %ld!\n", i);
	}

	transition_delete(&t);
	*/

	state_machine_t s = {0};
	state_machine_gen(&s, &e);

	state_machine_print(&s);

	capture_find_all(&s, cap, value, sizeof value);

	return 0;
}
