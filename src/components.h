#ifndef __COMPONENTS_H_HEADER_GUARD__
#define __COMPONENTS_H_HEADER_GUARD__

#include <stdio.h> // printf
#include <string.h> // strcmp
#include <stdbool.h> // bool, true, false
#include <stdlib.h> // setenv, malloc, calloc, realloc, free,
                    // getline

// Declarations
int setenv(const char *p_name, const char *p_val, int p_overwrite);
size_t getline(char **p_linePtr, size_t *p_n, FILE *p_stream);

#include <readline/readline.h> // readline, rl_set_signals
#include <readline/history.h> // add_history, using_history
#include <unistd.h>
#include <signal.h> // signal

#endif // __COMPONENTS_H_HEADER_GUARD__
