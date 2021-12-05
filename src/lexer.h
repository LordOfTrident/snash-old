#ifndef __LEXER_H_HEADER_GUARD__
#define __LEXER_H_HEADER_GUARD__

#include <stdbool.h> // bool, true, false
#include <stdlib.h> // malloc, realloc, free

#include "types.h"
#include "error.h"
#include "str.h"
#include "token.h"

#define LEXER_ID_CHARS \
	     'a': case 'A': case 'b': case 'B': \
	case 'c': case 'C': case 'd': case 'D': \
	case 'e': case 'E': case 'f': case 'F': \
	case 'g': case 'G': case 'h': case 'H': \
	case 'i': case 'I': case 'j': case 'J': \
	case 'k': case 'K': case 'l': case 'L': \
	case 'm': case 'M': case 'n': case 'N': \
	case 'o': case 'O': case 'p': case 'P': \
	case 'q': case 'Q': case 'r': case 'R': \
	case 's': case 'S': case 't': case 'T': \
	case 'u': case 'U': case 'u': case 'U': \
	case 'w': case 'W': case 'v': case 'V': \
	case 'y': case 'Y': case 'z': case 'Z': \
	case '_'

#define LEXER_DIGITS \
	     '0': case '1': case '2': case '3': \
	case '4': case '5': case '6': case '7': \
	case '8': case '9'

#define LEXER_WHITESPACES \
	     ' ': case '\t': case '\r'

// Lexer error system
#define LEX_ERROR(p_why, p_col, p_row) \
	g_lex_error.why  = p_why; \
	g_lex_error.col  = p_col; \
	g_lex_error.row  = p_row; \
	g_lex_error.hpnd = true

typedef struct {
	char *why;
	usize col;
	usize row;
	bool hpnd;
} t_lex_error;
extern t_lex_error g_lex_error;

void lex(t_str p_src);
static void lex_str(void);
static void lex_id(void);
static void lex_inc(void);
static void lex_set_tok_pos(void);

#endif // __LEXER_H_HEADER_GUARD__
