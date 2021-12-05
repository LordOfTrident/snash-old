#include "lexer.h"

// extern
t_lex_error g_lex_error;

// file-scoped
t_str g_src;
usize g_idx;

t_tok  g_tok;
t_toks g_toks;

usize g_col;
usize g_row;
bool  g_was_str;

void lex(t_str p_src) {
	g_lex_error.hpnd = false;
	g_src = p_src;
	g_idx = 0;

	g_tok.txt = str_new();
	g_col = 1;
	g_row = 1;

	g_was_str = false;

	toks_new(&g_toks);

	for (g_idx = 0; g_idx < g_src.len; lex_inc()) {
		switch (g_src.buf[g_idx]) {
		case '\n': ++ g_row;
		case '\t': case '\r': case ' ': break;
		case '"':
			lex_set_tok_pos();
			lex_str();
			if (g_lex_error.hpnd)
				goto l_finish;
			break;

		// For now, we allow all characters in an identifier
		default:
			lex_set_tok_pos();
			lex_id();
			if (g_lex_error.hpnd)
				goto l_finish;
		};
	};

l_finish: // No, labels arent forbidden. It is better to use a label to
	      // completely break out of a loop from inside a switch case instead
	      // of making a variable and an if statement to check for it inside
	      // the loop
	// START // DEBUG REMOVE LATER
	for (usize i = 0; i < g_toks.len; ++ i) {
		printf(
			"[%s at %lu:%lu]",
			g_toks.buf[i].txt.buf,
			(unsigned long)g_toks.buf[i].col,
			(unsigned long)g_toks.buf[i].row
		);
		fflush(stdout);
	};
	printf("\n");
	// END // DEBUG REMOVE LATER

	// g_src doesnt need to be freed because the address remained the same
	// as the address of the string passed in as the parameter, we will free
	// it elsewhere in the code
	str_free(&g_tok.txt);
	toks_free(&g_toks);
	if (g_error.hpnd)
		eprintf("FROM %s:%lu\n", ERR_FILE, (unsigned long)ERR_LINE);
};

static void lex_str(void) {
	lex_inc();

	g_was_str = true;
	for (; g_idx < g_src.len; lex_inc()) {
		switch (g_src.buf[g_idx]) {
		case '\n': goto l_error;
		case '"':
			// TODO: make "hello "world be lexed as "hello world"
			toks_push(&g_toks, g_tok);
			str_clear(&g_tok.txt);
			return;

		default: {
				// Converting a char to a string
				char ch[] = {g_src.buf[g_idx], '\0'};
				str_concat_into(&g_tok.txt, str(ch));
			};
		};
	};

l_error:
	LEX_ERROR("string not ended", g_tok.col, g_tok.row);
};

static void lex_id(void) {
	for (; g_idx < g_src.len; lex_inc()) {
		switch (g_src.buf[g_idx]) {
		case '\n': ++ g_row;
		case LEXER_WHITESPACES:
			goto l_push;

		default: {
				char ch[] = {g_src.buf[g_idx], '\0'};
				str_concat_into(&g_tok.txt, str(ch));
			};
		};
	};

l_push:
	toks_push(&g_toks, g_tok);
	str_clear(&g_tok.txt);
};

static void lex_inc(void) {
	++ g_idx; ++ g_col;
};

static void lex_set_tok_pos(void) {
	g_tok.row = g_row;
	g_tok.col = g_col;
};
