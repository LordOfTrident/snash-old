#ifndef __SNASH_H_HEADER_GUARD__
#define __SNASH_H_HEADER_GUARD__

#include <stdio.h> // printf
#include <stdbool.h> // bool, true, false
#include <stdlib.h> // setenv, getenv
#include <unistd.h> // getcwd
#include <signal.h> // sigaction, sigemptyset
#include <time.h> // time_t, tm, time, timeinfo, asctime
#include <readline/readline.h> // readline, rl_set_signals
#include <readline/history.h> // add_history, using_history

#include "types.h"
#include "error.h"
#include "str.h"

#include "colors.h"
#include "var.h"
#include "utils.h"

#include "version.h"

#include "lexer.h"

// Snash exitcodes
#define SNASH_EC_OK     0
#define SNASH_EC_IV_CMD 1 // Invalid cmd
#define SNASH_EC_NOT_IA 2 // Not interactive

// Snash prompts
#define SNASH_VAR_PDEF "PS1"
#define SNASH_VAR_PSEC "PS2"

// Snash vars
#define SNASH_VAR_HOSTNAME  "HOSTNAME"
#define SNASH_VAR_CURR_DIR  "PWD"
#define SNASH_VAR_SHELL     "SHELL"
#define SNASH_VAR_USER      "USER"
#define SNASH_VAR_HOME      "HOME"
#define SNASH_VAR_VERSION   "SNASH_VER"
#define SNASH_VAR_TIME      "TIME"
#define SNASH_VAR_TIME_HOUR "TIME_HOUR"
#define SNASH_VAR_TIME_MIN  "TIME_MIN"
#define SNASH_VAR_TIME_SEC  "TIME_SEC"

// Snash prompt special sequences macros
#define SNASH_PSS_CNT 7

#define SNASH_PSS_W_IDX     0
#define SNASH_PSS_H_IDX     1
#define SNASH_PSS_U_IDX     2
#define SNASH_PSS_V_IDX     3
#define SNASH_PSS_T_IDX     4
#define SNASH_PSS_BSBEG_IDX 5
#define SNASH_PSS_BSEND_IDX 6

// Snash built-in commands
#define SNASH_BIC_CNT 3

// Tools
#define __STR_HELPER__(p_x) #p_x
#define TO_STR(p_x) __STR_HELPER__(p_x)

typedef struct {
	t_str seq;
	t_str rep;
} t_snash_pss;

extern u8    g_snash_exitcode;
extern t_str g_snash_path;
// Main snash functions
void snash_init(void);
void snash_main_loop(void);
void snash_finish(void);

// Rest of snash functions
static void snash_update(void);
static void snash_set_env_vars(void);
t_str snash_get_prompt(void);
bool  snash_read_params(int p_argc, const char *p_argv[]);

#endif // __SNASH_H_HEADER_GUARD__
