#ifndef __ERROR_H_HEADER_GUARD__
#define __ERROR_H_HEADER_GUARD__

#include <stdio.h> // fprintf, fflush
#include <stdlib.h> // abort
#include <stdbool.h> // bool, true, false

#include "types.h"

#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#define eflush()     fflush(stderr)

// Macros for more consistent names
#define ERR_FILE __FILE__
#define ERR_FUNC (char*)__func__
#define ERR_LINE __LINE__

#define ERR_WATCH __err_line__ = ERR_LINE
#define ERR_WATCH_LINE __err_line__
#define ERR_WATCH_INIT usize __err_line__ = 0

#define ERR_SET_G_ERROR(p_why) \
	g_error.why  = p_why; \
	g_error.file = ERR_FILE; \
	g_error.func = ERR_FUNC; \
	g_error.line = ERR_WATCH_LINE; \
	g_error.hpnd = true;

typedef struct {
	char *why;
	char *func;
	char *file;
	usize line;
	bool hpnd;
} t_error;
extern t_error g_error;

void error(void);

#endif // __ERROR_H_HEADER_GUARD__
