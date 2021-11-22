#ifndef __SNASH_H_HEADER_GUARD__
#define __SNASH_H_HEADER_GUARD__

#include "components.h"
#include "types.h"
#include "colors.h"
#include "utils.h"

#define SNASH_VERSION_MAJOR 1
#define SNASH_VERSION_MINOR 0
#define SNASH_VERSION_PATCH 0

// Error exitcodes of functions
#define SNASH_NO_ERROR      0
#define SNASH_GENERIC_ERROR 1
#define SNASH_MALLOC_ERROR  2
#define SNASH_REALLOC_ERROR 3

// Snash exitcodes
#define SNASH_EC_OK          0
#define SNASH_EC_INVALID_CMD 1

// Snash states
#define SNASH_ST_NOTRUNNING 0
#define SNASH_ST_RUNNING    1
#define SNASH_ST_HASTOEXIT  2

typedef struct {
	char *name;
	char *val;
} Snash_Var;

typedef struct {
	Snash_Var *buf;
	usize len;
} Snash_Vars;

extern bool Snash_state;
extern char *Snash_prompt;
extern Snash_Vars Snash_vars;
extern u8 Snash_exitcode;
extern char *Snash_path;

u8    Snash_SetVar(
	Snash_Vars *p_vars,
	const char *p_name,
	const char *p_val
);
char *Snash_GetVar(Snash_Vars *p_vars, const char *p_name);

void Snash_Init(void);
void Snash_MainLoop(void);
void Snash_Finish(void);

void Snash_InternalError(const u8 p_errId, const char* p_why);
bool Snash_ReadParams(const int p_argc, const char *p_argv[]);
void Snash_SignalHandler(const int p_signal);

#endif // __SNASH_H_HEADER_GUARD__
