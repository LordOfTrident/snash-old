#ifndef __SNASH_H_HEADER_GUARD__
#define __SNASH_H_HEADER_GUARD__

#include <stdio.h> // printf, fprintf
#include <stdbool.h> // bool, true, false
#include <stdlib.h> // setenv, getenv, malloc, realloc, free,
                    // getline
#include <unistd.h> // getcwd
#include <signal.h> // sigaction, sigemptyset
#include <readline/readline.h> // readline, rl_set_signals
#include <readline/history.h> // add_history, using_history

#include "types.h"
#include "error.h"
#include "str.h"

#include "colors.h"
#include "var.h"
#include "utils.h"

#include "version.h"

// Snash exitcodes
#define SNASH_EC_OK          0
#define SNASH_EC_INVALID_CMD 1

// Snash prompts
#define SNASH_VAR_PDEF "PS1"
#define SNASH_VAR_PSEC "PS2"

// Snash vars
#define SNASH_VAR_HOSTNAME "HOSTNAME"
#define SNASH_VAR_CURR_DIR "PWD"
#define SNASH_VAR_SHELL    "SHELL"
#define SNASH_VAR_USER     "USER"
#define SNASH_VAR_VERSION  "SNASH_VER"

// Snash prompt special sequences macros
#define SNASH_PSS_CNT 4

#define SNASH_PSS_W_IDX 0
#define SNASH_PSS_H_IDX 1
#define SNASH_PSS_U_IDX 2
#define SNASH_PSS_V_IDX 3

// Tools
#define SNASH_STRINGIFY(p_x) #p_x
#define SNASH_VERSION \
	SNASH_STRINGIFY(SNASH_VERSION_MAJOR)"." \
	SNASH_STRINGIFY(SNASH_VERSION_MINOR)"." \
	SNASH_STRINGIFY(SNASH_VERSION_PATCH)

typedef enum {
	snash_runst_not_running,
	snash_runst_running
} t_snash_runst;

typedef struct {
	t_str seq;
	t_str rep;
} t_snash_pss;

extern t_snash_pss g_snash_pss[SNASH_PSS_CNT];
extern t_str  g_snash_path;
extern u8     g_snash_exitcode;
extern t_vars g_snash_vars;
extern t_snash_runst g_snash_runst;

// Main snash functions
void snash_init(void);
void snash_main_loop(void);
void snash_finish(void);

// Rest of snash functions
void  snash_update_vars(void);
t_str snash_get_prompt(void);
bool  snash_read_params(int p_argc, const char *p_argv[]);
void  snash_signal_handler(int p_signal);

#endif // __SNASH_H_HEADER_GUARD__
