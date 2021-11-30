#ifndef __VAR_H_HEADER_GUARD__
#define __VAR_H_HEADER_GUARD__

#include <stdlib.h> // malloc, realloc, free

#include "types.h"
#include "error.h"
#include "str.h"

typedef struct {
	t_str name;
	t_str val;
} t_var;

typedef struct {
	t_var *buf;
	usize len;
} t_vars;

void  vars_init(t_vars *p_vars);
void  vars_free(t_vars *p_vars);
t_str vars_getvar(t_vars *p_vars, t_str p_name);
void  vars_setvar(t_vars *p_vars, t_str p_name, t_str p_val);

#endif // __VAR_H_HEADER_GUARD__
