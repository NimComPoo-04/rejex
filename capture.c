#include "capture.h"

static int relative_finder(state_machine_t *s,
		capture_func_t func,
		const char *str,
		int len,
		int quit,
		int start_state)
{
	int current_sate = start_state;

	int si = 0;
	int ei = 0;

	token_t val = {.type = NORMAL, .value = 0};
	token_t any = {.type = ANY,    .value = '.'};
	token_t end = {.type = END,    .value = 0};

	for(int i = 0; i < len; i++)
	{
		si = i;
		ei = i;
		current_sate = start_state;

		while(current_sate != -1 && current_sate != -2 && ei < len)
		{
			val.value = str[ei++];
			int k = transition_fetch(&s->states.states_vector[current_sate].next_state, val);

			if(k == -1)
				k = transition_fetch(&s->states.states_vector[current_sate].next_state, any);

			if(k == -1)
				k = transition_fetch(&s->states.states_vector[current_sate].next_state, end); 

			current_sate = k;
		}

		if(current_sate == -2)
		{
			ei--; // releasing the one collected by end
			func(si, ei);
			i = ei - (si != ei);
			if(quit)
				return 1;
		}
	}

	return 0;
}

// TODO: thread this guy out to oblivion
static int capfind(state_machine_t *s, capture_func_t func, const char *str, int len, int quit)
{
	if(relative_finder(s, func, str, len, quit, 0) && quit)
		return 1;

	// Incoming
	if(s->headers.values)
	{
		for(int i = 0; i < s->headers.len; i++)
		{
			if(relative_finder(s, func, str, len, quit, s->headers.values[i]) && quit)
				return 1;
		}
	}

	return 0;
}

int capture_find(state_machine_t *s, capture_func_t func, const char *str, int len)
{
	return capfind(s, func, str, len, 1);
}

int capture_find_all(state_machine_t *s, capture_func_t func, const char *str, int len)
{
	return capfind(s, func, str, len, 0);
}
