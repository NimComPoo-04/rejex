#ifndef _CAPTURE_H_
#define _CAPTURE_H_

#include "state.h"

typedef void(*capture_func_t)(int start, int end);

int capture_find(state_machine_t *, capture_func_t, const char *, int);
int capture_find_all(state_machine_t *, capture_func_t, const char *, int);

#endif
