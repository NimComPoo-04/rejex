#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state.h"

state_t *state_vector_insert(state_vector_t *s, int matches_null)
{
	if(s->state_vector_size >= s->state_vector_cap)
	{
		s->state_vector_cap = 2 * s->state_vector_cap + !s->state_vector_cap * 64;
		s->states_vector = realloc(s->states_vector, sizeof(state_t) * s->state_vector_cap);
	}

	memset(&(s->states_vector[s->state_vector_size++]), 0, sizeof(state_t));
	s->states_vector[s->state_vector_size - 1].matches_null = matches_null;
	return &(s->states_vector[s->state_vector_size - 1]);
}

static void insert_header(state_machine_t *s, int state_pos)
{
	if(s->headers.len >= s->headers.size)
	{
		s->headers.size = (!s->headers.size * 64 + 2 * s->headers.size);
		s->headers.values = realloc(s->headers.values,
				(!s->headers.size * 64 + s->headers.size) * sizeof s->headers);
	}

	s->headers.values[s->headers.len++] = state_pos;
}

int state_machine_gen(state_machine_t *s, const expression_t *e)
{
	int si = 0;
	size_t i = 0;

	state_t *prev = 0;
	while(i < e->token_vector_size)
	{
		switch(e->token_vector[i].type)
		{
			case NORMAL:
			case NEWLINE:
			case ANY:
				if(i + 1 < e->token_vector_size)
				{
					i++;
					switch(e->token_vector[i].type)
					{
						case NORMAL: case ANY: case NEWLINE: case END:
							i--;
							break;

						case CONTROL:
							switch(e->token_vector[i].value)
							{
								case '?':
									prev = state_vector_insert(&s->states, 1);

									transition_emplace(&prev->next_state, e->token_vector[i-1], si + 1);

									for(int j = si; j >= 0 && s->states.states_vector[j].matches_null; j--)
									{
										if(e->token_vector[i+1].type == END)
											transition_emplace(&s->states.states_vector[j].next_state,
													e->token_vector[i+1], -1);
										else
											transition_emplace(&s->states.states_vector[j].next_state,
													e->token_vector[i+1], si + 2);
									}

									si++;
									continue;

									// a+ => aa*
								case '+':
									prev = state_vector_insert(&s->states, 0);
									transition_emplace(&prev->next_state, e->token_vector[i-1], ++si);

									// FIXME: removing the fallthrough warning, do something better
									goto A;

								case '*':
A:
									prev = state_vector_insert(&s->states, 1);

									transition_emplace(&prev->next_state, e->token_vector[i-1], si);
									transition_emplace(&prev->next_state, e->token_vector[i-1], si + 1);

									for(int j = si; j >= 0 && s->states.states_vector[j].matches_null; j--)
									{
										if(e->token_vector[i+1].type == END)
											transition_emplace(&s->states.states_vector[j].next_state,
													e->token_vector[i+1], -2);
										else
											transition_emplace(&s->states.states_vector[j].next_state,
													e->token_vector[i+1], si + 2);
									}

									si++;
									continue;

								case '|':
									i--;
									break;
							}
					}
				}
				prev = state_vector_insert(&s->states, 0);
				transition_emplace(&prev->next_state, e->token_vector[i], ++si);
				break;

			case CONTROL:
				if(e->token_vector[i].value == '|')
				{
					prev = state_vector_insert(&s->states, 0);
					transition_emplace(&prev->next_state, (token_t){.type = END, .value = 0}, -2);
					++si;
					insert_header(s, si);
				}
				break;

			case END:
				prev = state_vector_insert(&s->states, 0);
				transition_emplace(&prev->next_state, e->token_vector[i], -2);
				++si;
				break;
		}
		i++;
	}

	return 0;
}

void state_machine_dinit(state_machine_t *s)
{
	for(size_t i = 0; i < s->states.state_vector_size; i++)
	{
		transition_delete(&(s->states.states_vector[i].next_state));
	}
}

static void foreach(token_t key, int value)
{
	printf("%-16s : %5d | ", token_to_str(key), value);
}

void state_machine_print(state_machine_t *s)
{
	for(size_t i = 0; i < s->states.state_vector_size; i++)
	{
		printf("%5d | ", (int)i);
		token_avl_tree_iterate(s->states.states_vector[i].next_state.token_state_map, foreach);
		puts("");
	}

	for(int i = 0; i < s->headers.len; i++)
	{
		printf("%3d ", s->headers.values[i]);
	}
	puts("");
}
