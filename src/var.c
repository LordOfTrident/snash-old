#include "var.h"

void vars_init(t_vars *p_vars) {
	ERR_WATCH_INIT; ERR_WATCH;
	p_vars->buf = (t_var*)malloc(sizeof(t_var)); ERR_WATCH;
	if (p_vars->buf == NULL) {
		ERR_SET_G_ERROR("malloc fail");
		return;
	};
	p_vars->len = 1;

	p_vars->buf[0].name = str_new();
	p_vars->buf[0].val  = str_new();
};

void vars_free(t_vars *p_vars) {
	ERR_WATCH_INIT; ERR_WATCH;
	if (p_vars == NULL) {
		ERR_SET_G_ERROR("p_vars is NULL");
		return;
	};

	for (usize i = 0; i < p_vars->len; ++ i) {
		if (p_vars->buf[i].name.buf != NULL)
			str_free(&p_vars->buf[i].name);

		if (p_vars->buf[i].val.buf != NULL)
			str_free(&p_vars->buf[i].val);
	};

	free(p_vars->buf);
	p_vars->buf = NULL;
	p_vars->len = 0;
};

void vars_setvar(t_vars *p_vars, t_str p_name, t_str p_val) {
	ERR_WATCH_INIT; ERR_WATCH;
	if (p_vars == NULL) {
		ERR_SET_G_ERROR("p_vars is NULL");
		return;
	} else if (p_name.buf == NULL) {
		ERR_SET_G_ERROR("p_name.buf is NULL");
		return;
	} else if (p_val.buf == NULL) {
		ERR_SET_G_ERROR("p_val.buf is NULL");
		return;
	} else if (p_vars->buf == NULL) {
		ERR_SET_G_ERROR("p_vars->buf is NULL");
		return;
	};

	for (usize i = 0; i < p_vars->len; ++ i) {
		if (str_comp(p_vars->buf[i].name, p_name))
			return str_copy_into(&p_vars->buf[i].val, p_val);
	};

	++ p_vars->len;
	void *tmp = (t_var*)realloc(
		p_vars->buf,
		sizeof(t_var) * p_vars->len
	); ERR_WATCH;
	if (tmp == NULL) {
		free(p_vars->buf);
		ERR_SET_G_ERROR("realloc fail");
		return;
	};
	p_vars->buf = (t_var*)tmp;

	usize idx = p_vars->len - 1;
	p_vars->buf[idx].name = str_copy(p_name);
	p_vars->buf[idx].val  = str_copy(p_val);
};

t_str vars_getvar(t_vars *p_vars, t_str p_name) {
	ERR_WATCH_INIT; ERR_WATCH;
	if (p_vars->buf == NULL) {
		ERR_SET_G_ERROR("p_vars->buf is NULL");
		return STR_NULL;
	} else if (p_name.buf == NULL) {
		ERR_SET_G_ERROR("p_name.buf is NULL");
		return STR_NULL;
	};

	for (usize i = 0; i < p_vars->len; ++ i)
		if (str_comp(p_vars->buf[i].name, p_name))
			return p_vars->buf[i].val;

	return STR_NULL;
};
