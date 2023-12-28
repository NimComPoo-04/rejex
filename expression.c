#include <stdio.h>
#include <stdlib.h>

#include "expression.h"

int expression_tokenize(expression_t *e)
{
	e->token_vector = 0;
	e->token_vector_size = 0; e->token_vector_cap = 0;

	for(size_t i = 0; i <= e->expr_str_size; i++)
	{
		token_t tok = {.type = NORMAL, .value = e->expr_str[i]};

		switch(e->expr_str[i])
		{
			case 0:
				tok.type = END;
				break;

			case '?': case '*': case '+': 
			case '(': case ')': case '|': 
				tok.type = CONTROL;
				break;

			case '.': 
				tok.type = ANY;
				break;

			case '\\':
				if(i + 1 > e->expr_str_size)
					return 1;
				switch(e->expr_str[++i])
				{
					case 'n': case 'r':
						tok.type = NEWLINE;
						tok.value = e->expr_str[i] == 'r' ? '\r' : '\n';
						break;

					default:
						tok.value = e->expr_str[i];
				}
				break;

			case '\n': case '\r':
				tok.type = NEWLINE;
				break;
		}

		if(e->token_vector_cap <= e->token_vector_size)
		{
			e->token_vector_cap = e->token_vector_cap ? e->token_vector_cap* 2 : 64;
			void *tmp = realloc(e->token_vector, e->token_vector_cap * sizeof(token_t));

			// Not GOOD
			if(tmp == NULL) return -1;

			/*
			if(e->token_vector != tmp && e->token_vector != 0)
				fprintf(stderr, "MOVED!!\n");
			*/

			e->token_vector = tmp;
		}

		e->token_vector[e->token_vector_size++] = tok;
	}

	return 0;
}

char *token_to_str(token_t e)
{
	static char buffer[256];

	switch(e.type)
	{
		case END:     sprintf(buffer, "[ END ]"); break;
		case ANY:     sprintf(buffer, "[ ANY ]"); break;
		case NEWLINE: sprintf(buffer, "[ NEWLINE ]"); break;

		case NORMAL:  sprintf(buffer, "[ NORMAL : %c ]", e.value); break;
		case CONTROL: sprintf(buffer, "[ CONTROL : %c ]", e.value); break;
	}

	return buffer;
}

void expression_print(expression_t *e)
{
	for(size_t i = 0; i < e->token_vector_size; i++)
	{
		printf(token_to_str(e->token_vector[i]));
	}
	puts("");
}
