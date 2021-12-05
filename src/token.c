#include "token.h"

void toks_new(t_toks *p_toks) {
	ERR_WATCH_INIT;
	p_toks->len = 0;
	p_toks->buf = (t_tok*)malloc(sizeof(t_tok)); ERR_WATCH;
	if (p_toks->buf == NULL) {
		ERR_SET_G_ERROR("malloc fail");
		return;
	};
};

void toks_push(t_toks *p_toks, t_tok p_tok) {
	ERR_WATCH_INIT;
	++ p_toks->len;
	void *tmp = realloc(p_toks->buf, sizeof(t_tok) * p_toks->len);
	if (tmp == NULL) {
		free(p_toks->buf);
		ERR_SET_G_ERROR("realloc fail");
		return;
	};
	p_toks->buf = (t_tok*)tmp;
	p_toks->buf[p_toks->len - 1] = (p_tok);
	// We also need to make the buffer a copy of the original to
	// prevent double frees!
	p_toks->buf[p_toks->len - 1].txt = str_copy(p_tok.txt);
};

void toks_free(t_toks *p_toks) {
	ERR_WATCH_INIT;
	if (p_toks->buf == NULL) {
		ERR_SET_G_ERROR("p_toks->buf is NULL");
		return;
	};

	for (usize i = 0; i < p_toks->len; ++ i)
		str_free(&p_toks->buf[i].txt);
	if (g_error.hpnd) {
		eprintf("FROM %s:%lu\n", ERR_FILE, (unsigned long)ERR_LINE);
		return;
	};

	free(p_toks->buf);
};
