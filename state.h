#ifndef _STATE_H_
#define _STATE_H_

#include "transition.h"

// XXX: This struct may not actually be needed but imma keep it just incase we decide
// to include more info with the state itself.
typedef struct
{
	transitions_t next_state; // next state depends on input
	int matches_null;	 // check if the state is such that it can be reached by null
} state_t;

typedef struct
{
	state_t *states_vector;
	size_t state_vector_size;
	size_t state_vector_cap;
} state_vector_t;

state_t *state_vector_insert(state_vector_t *s, int);

typedef struct
{
	state_vector_t states;
	struct
	{
		int *values;
		int size;
		int len;
	} headers; // holds other heads of expression for or statement if any
} state_machine_t;

int  state_machine_gen(state_machine_t *, const expression_t *);
void state_machine_delete(state_machine_t *); 

void state_machine_print(state_machine_t *);

#endif
