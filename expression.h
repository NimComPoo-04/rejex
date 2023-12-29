#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include <stdint.h>
#include <stddef.h>

enum
{
	END,
	NORMAL,
	ANY,
	NEWLINE,
	CONTROL,
	INTERNAL,
};

typedef struct
{
	uint16_t type;	
	uint16_t value;	
} token_t;

typedef struct
{
	// Token Vector
	token_t *token_vector;
	size_t token_vector_size;
	size_t token_vector_cap;

	const char *expr_str;
	size_t expr_str_size;
} expression_t;

int expression_tokenize(expression_t *e);

char *token_to_str(token_t e);
void expression_print(expression_t *e);

#endif
