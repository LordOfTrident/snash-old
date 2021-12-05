#ifndef __TOKEN_H_HEADER_GUARD__
#define __TOKEN_H_HEADER_GUARD__

#include <stdlib.h> // malloc, realloc, free

#include "types.h"
#include "error.h"
#include "str.h"


typedef struct {
	t_str txt;
	usize row;
	usize col;
} t_tok;

typedef struct {
	t_tok *buf;
	usize len;
} t_toks;

void toks_new(t_toks *p_toks);
void toks_push(t_toks *p_toks, t_tok p_tok);
void toks_free(t_toks *p_toks);

#endif // __TOKEN_H_HEADER_GUARD__
