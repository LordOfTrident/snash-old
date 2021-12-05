#include "error.h"

t_error g_error = {
	NULL,  // why
	NULL,  // func
	NULL,  // file
	0,     // line
	false, // hpnd
};

void error(void) {
	eprintf(
		"SNASH fatal error:\n"
		"  why: %s\n"
		"  file: %s\n"
		"  func: %s\n"
		"  line: %lu\n"
		"This may be an internal bug, please report it\n",
		g_error.why,
		g_error.file,
		g_error.func,
		(unsigned long)g_error.line
	);

	eflush();
	abort();
};
