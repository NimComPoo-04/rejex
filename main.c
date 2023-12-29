#include <stdio.h>

#include "expression.h"
#include "transition.h"
#include "state.h"
#include "capture.h"

char expr[] = ".";

char value[4096] = {0};

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

	FILE *f = fopen("test.txt", "rb+");
	// the size of the test file for now is 3643 byes so it fits
	fread(value, sizeof value, 1, f);
	fclose(f);

	state_machine_t s = {0};
	state_machine_gen(&s, &e);

	state_machine_print(&s);

	capture_find_all(&s, cap, value, 3683);

	return 0;
}
