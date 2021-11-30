#include "str.h"

t_str str(char *p_buf) {
	return (t_str){p_buf, strlen(p_buf)};
};

t_str str_new(void) {
	ERR_WATCH_INIT;
	char *buf = (char*)malloc(1); ERR_WATCH;
	if (buf == NULL) {
		ERR_SET_G_ERROR("malloc fail");
		return STR_NULL;
	};

	buf[0] = '\0';
	return (t_str){buf, 0};
};

void str_free(t_str *p_mstr) {
	ERR_WATCH_INIT; ERR_WATCH;
	if (p_mstr->buf == NULL) {
		ERR_SET_G_ERROR("p_mstr->buf is NULL");
		return;
	};

	free(p_mstr->buf);
	p_mstr->buf = NULL;
	p_mstr->len = 0;
};

t_str str_copy(t_str p_str) {
	ERR_WATCH_INIT; ERR_WATCH;
	if (p_str.buf == NULL) {
		ERR_SET_G_ERROR("p_str.buf is NULL");
		return STR_NULL;
	};

	char *buf = (char*)malloc(p_str.len + 1); ERR_WATCH;
	if (buf == NULL) {
		ERR_SET_G_ERROR("malloc fail");
		return STR_NULL;
	};

	memcpy(buf, p_str.buf, p_str.len);
	buf[p_str.len] = '\0'; // Dont forget the null terminator just in
	                       // case (for functions which depend on it)
	return (t_str){buf, p_str.len};
};

void str_copy_into(t_str *p_mstr, t_str p_str) {
	ERR_WATCH_INIT; ERR_WATCH;
	if (p_mstr->buf == NULL) {
		ERR_SET_G_ERROR("p_mstr->buf is NULL");
		return;
	} else if (p_str.buf == NULL) {
		ERR_SET_G_ERROR("p_str.buf is NULL");
		return;
	};

	void *tmp = realloc(p_mstr->buf, p_str.len + 1); ERR_WATCH;
	if (tmp == NULL) {
		str_free(p_mstr); // Also free the original buffer
		ERR_SET_G_ERROR("realloc fail");
		return;
	};
	p_mstr->buf = (char*)tmp;

	memcpy(p_mstr->buf, p_str.buf, p_str.len);
	p_mstr->buf[p_str.len] = '\0';
	p_mstr->len = p_str.len;
};

void str_clear(t_str *p_mstr) {
	ERR_WATCH_INIT; ERR_WATCH;
	if (p_mstr->buf == NULL) {
		ERR_SET_G_ERROR("p_mstr->buf is NULL");
		return;
	};

	void *tmp = realloc(p_mstr->buf, 1); ERR_WATCH;
	if (tmp == NULL) {
		str_free(p_mstr);
		ERR_SET_G_ERROR("realloc fail");
		return;
	};

	p_mstr->buf = (char*)tmp;
	p_mstr->buf[0] = '\0';
	p_mstr->len = 0;
};

void str_resize(t_str *p_mstr, usize p_new_len) {
	ERR_WATCH_INIT; ERR_WATCH;
	if (p_mstr->buf == NULL) {
		ERR_SET_G_ERROR("p_mstr->buf is NULL");
		return;
	};

	void *tmp = realloc(p_mstr->buf, p_new_len + 1); ERR_WATCH;
	if (tmp == NULL) {
		str_free(p_mstr);
		ERR_SET_G_ERROR("realloc fail");
		return;
	};
	p_mstr->buf = (char*)tmp;

	if (p_mstr->len < p_new_len) {
		for (usize i = p_mstr->len; i <= p_new_len; ++ i)
			p_mstr->buf[i];
	} else // The for loop includes the last null terminator
		p_mstr->buf[p_new_len] = '\0';

	p_mstr->len = p_new_len;
};

bool str_comp(t_str p_stra, t_str p_strb) {
	return strcmp(p_stra.buf, p_strb.buf) == 0;
};

t_str str_concat(t_str p_stra, t_str p_strb) {
	ERR_WATCH_INIT; ERR_WATCH;
	if (p_stra.buf == NULL) {
		ERR_SET_G_ERROR("p_stra.buf is NULL");
		return STR_NULL;
	} else if (p_strb.buf == NULL) {
		ERR_SET_G_ERROR("p_strb.buf is NULL");
		return STR_NULL;
	};

	usize new_len = p_stra.len + p_strb.len;
	char *buf = malloc(new_len + 1); ERR_WATCH;
	if (buf == NULL) {
		ERR_SET_G_ERROR("malloc fail");
		return STR_NULL;
	};

	memcpy(buf, p_stra.buf, p_stra.len);
	memcpy(buf + p_stra.len, p_strb.buf, p_strb.len);
	buf[new_len] = '\0';

	return (t_str){buf, new_len};
};

void str_concat_into(t_str *p_mstr, t_str p_str) {
	ERR_WATCH_INIT; ERR_WATCH;
	if (p_mstr->buf == NULL) {
		ERR_SET_G_ERROR("p_mstr->buf is NULL");
		return;
	} else if (p_str.buf == NULL) {
		ERR_SET_G_ERROR("p_str.buf is NULL");
		return;
	};

	usize new_len = p_mstr->len + p_str.len;
	void *tmp = realloc(p_mstr->buf, new_len + 1); ERR_WATCH;
	if (tmp == NULL) {
		str_free(p_mstr);
		ERR_SET_G_ERROR("realloc fail");
		return;
	};
	p_mstr->buf = (char*)tmp;

	memcpy(p_mstr->buf + p_mstr->len, p_str.buf, p_str.len);
	p_mstr->len = new_len;
	p_mstr->buf[new_len] = '\0';
};

t_str str_substr(t_str p_str, usize p_start, usize p_len) {
	ERR_WATCH_INIT; ERR_WATCH;
	if (p_str.buf == NULL) {
		ERR_SET_G_ERROR("p_str.buf is NULL");
		return STR_NULL;
	} else if (p_start > p_str.len || p_str.len < p_start + p_len) {
		ERR_SET_G_ERROR("invalid p_len/p_start");
		return STR_NULL;
	} else if (p_len == 0)
		return str_copy(str(""));

	char *buf = (char*)malloc(p_len + 1); ERR_WATCH;
	if (buf == NULL) {
		ERR_SET_G_ERROR("malloc fail");
		return STR_NULL;
	};
	memcpy(buf, p_str.buf + p_start, p_len);

	buf[p_len] = '\0';
	return (t_str){buf, p_len};
};

void str_substr_into(t_str *p_mstr, usize p_start, usize p_len) {
	ERR_WATCH_INIT; ERR_WATCH;
	if (p_mstr->buf == NULL) {
		ERR_SET_G_ERROR("p_mstr->buf is NULL");
		return;
	} else if (p_start > p_mstr->len || p_mstr->len < p_start + p_len) {
		ERR_SET_G_ERROR("invalid p_len/p_start");
		return;
	};

	// Copy the substring to the start of the string
	if (p_start != 0)
		memcpy(p_mstr->buf, p_mstr->buf + p_start, p_len);

	// Then cut it
	void *tmp = realloc(p_mstr->buf, p_len + 1); ERR_WATCH;
	if (tmp == NULL) {
		str_free(p_mstr);
		ERR_SET_G_ERROR("realloc fail");
		return;
	};

	p_mstr->buf = (char*)tmp;
	p_mstr->buf[p_len] = '\0';
	p_mstr->len = p_len;
};

t_str str_replace(t_str p_str, t_str p_torep, t_str p_rep) {
	ERR_WATCH_INIT; ERR_WATCH;
	if (p_str.buf == NULL) {
		ERR_SET_G_ERROR("p_str.buf is NULL");
		return STR_NULL;
	} else if (p_torep.buf == NULL) {
		ERR_SET_G_ERROR("p_torep.buf is NULL");
		return STR_NULL;
	};

	if (p_rep.buf == NULL)
		str_clear(&p_rep);

	// Count how many times the substring occurs in
	// the string
	usize cnt; // Count, not cunt
	char *ins = p_str.buf;
	for (cnt = 0; ins < p_str.buf + p_str.len; ++ cnt, ins += p_rep.len) {
		if ((ins = strstr(ins, p_torep.buf)) == NULL)
			break;
	};

	// It is so we can just allocate the needed memory all at once
	// instead of reallocating all the time which would be slower
	// because allocation on the heap isnt cheap
	usize len = p_str.len + (p_rep.len - p_torep.len) * cnt;
	char *buf = (char*)malloc(len + 1); ERR_WATCH;
	if (buf == NULL) {
		ERR_SET_G_ERROR("malloc fail");
		return STR_NULL;
	};

	ins = p_str.buf;
	char *tmp = buf;
	while (cnt --) {
		ins = strstr(p_str.buf, p_torep.buf);
		usize pos = ins - p_str.buf;

		tmp = strncpy(tmp, p_str.buf, pos) + pos;
		tmp = strcpy(tmp, p_rep.buf) + p_rep.len;
		p_str.buf += pos + p_torep.len; // p_str is a copy of the original
		                              // string struct, we dont care about
		                              // modifying it
	};

	// Dont forget to add the last part
	*tmp = '\0';
	strcat(tmp, p_str.buf);
	return (t_str){buf, len};
};

void str_replace_into(t_str *p_mstr, t_str p_torep, t_str p_rep) {
	ERR_WATCH_INIT; ERR_WATCH;
	if (p_mstr->buf == NULL) {
		ERR_SET_G_ERROR("p_mstr->buf is NULL");
		return;
	} else if (p_torep.buf == NULL) {
		ERR_SET_G_ERROR("p_torep.buf is NULL");
		return;
	};

	if (p_rep.buf == NULL)
		str_clear(&p_rep);

	usize cnt;
	char *ins = p_mstr->buf;
	for (cnt = 0; ins < p_mstr->buf + p_mstr->len; ++ cnt, ins += p_rep.len) {
		if ((ins = strstr(ins, p_torep.buf)) == NULL)
			break;
	};

	// reallocing is not really ideal here so we just
	// malloc new memory
	usize len = p_mstr->len + (p_rep.len - p_torep.len) * cnt;
	char *buf = (char*)malloc(len + 1); ERR_WATCH;
	if (buf == NULL) {
		ERR_SET_G_ERROR("malloc fail");
		return;
	};

	ins = p_mstr->buf;
	char *orig = p_mstr->buf; // We dont want to modify the string
	                          // struct now because we take a pointer
	                          // to it
	char *tmp = buf;
	while (cnt --) {
		ins = strstr(orig, p_torep.buf);
		usize pos = ins - orig;

		tmp = strncpy(tmp, orig, pos) + pos;
		tmp = strcpy(tmp, p_rep.buf) + p_rep.len;
		orig += pos + p_torep.len;
	};

	strcat(tmp, orig);
	str_free(p_mstr); // Free the original string
	p_mstr->buf = buf;
	p_mstr->len = len;
};
