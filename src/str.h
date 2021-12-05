#ifndef __STR_H_HEADER_GUARD__
#define __STR_H_HEADER_GUARD__

#include <string.h> // memcpy, strlen, strcmp, strstr
#include <stdlib.h> // malloc, realloc, free
#include <stdbool.h> // bool, true, false

#include "types.h"
#include "error.h"

// Strings to make life easier

typedef struct {
	char *buf;
	usize len;
} t_str;

#define STR_NULL ((t_str){NULL, 0})
// This macro caused a bug in snash input and i just realised why,
// i replaced it with a function.
// #define STR(p_buf) ((t_str){p_buf, strlen(p_buf)})

// Tho it is still sometimes handy so a modified version is defined:
#define STR(p_buf, p_len) ((t_str){p_buf, p_len})

#define STR_SUBSTR_LEN_REST ((usize)-1)
#define STR_REPLACE_CNT_ALL ((usize)-1)

t_str str(char *p_buf);
t_str str_new(void);
void  str_free(t_str *p_mstr);

t_str str_copy(t_str p_str);
void  str_copy_into(t_str *p_mstr, t_str p_str);

void str_clear(t_str *p_mstr);
void str_resize(t_str *p_mstr, usize p_new_len);
bool str_comp(t_str p_stra, t_str p_strb);
bool str_comp_entire(t_str p_stra, t_str p_strb);

t_str str_concat(t_str p_stra, t_str p_strb);
void  str_concat_into(t_str *p_mstr, t_str p_str);

t_str str_substr(t_str p_str, usize p_start, usize p_len);
void  str_substr_into(t_str *p_mstr, usize p_start, usize p_len);
t_str str_replace(t_str p_str, t_str p_torep, t_str p_rep, usize p_cnt);
void  str_replace_into(t_str *p_mstr, t_str p_torep, t_str p_rep, usize p_cnt);

#endif // __STR_H_HEADER_GUARD__
