#include "snash.h"

bool Snash_state = SNASH_ST_NOTRUNNING;
char *Snash_prompt = NULL;
Snash_Vars Snash_vars;
u8 Snash_exitcode = SNASH_EC_OK;
char *Snash_path;

// Snash variables list functions
u8 Snash_SetVar(
	Snash_Vars *p_vars,
	const char *p_name,
	const char *p_val
) {
	if (p_vars == NULL)
		return SNASH_GENERIC_ERROR;

	if (p_vars->buf == NULL) {
		p_vars->buf = (Snash_Var*)malloc(sizeof(Snash_Var));
		p_vars->len = 1;

		if (p_vars->buf == NULL)
			return SNASH_MALLOC_ERROR;

		goto l_setValues;
	};

	for (usize i = 0; i < p_vars->len; ++ i) {
		if (strcmp(p_vars->buf[i].name, p_name) == 0) {
			void *tmp = realloc(
				p_vars->buf[i].val,
				strlen(p_val) + 1
			);
			if (tmp == NULL) {
				free(p_vars->buf[i].val);
				return SNASH_REALLOC_ERROR;
			};


			p_vars->buf = (Snash_Var*)tmp;
			strcpy(p_vars->buf[i].val, p_val);
			return SNASH_NO_ERROR;
		};
	};

	++ p_vars->len;
	void *tmp = (Snash_Var*)realloc(
		p_vars->buf,
		sizeof(Snash_Var) * p_vars->len
	);

	if (tmp == NULL) {
		free(p_vars->buf);
		return SNASH_REALLOC_ERROR;
	};

	p_vars->buf = (Snash_Var*)tmp;

l_setValues:; // The semicolon is here to prevent a c99 error
	usize idx = p_vars->len - 1;
	p_vars->buf[idx].name = malloc(strlen(p_name) + 1);
	if (p_vars->buf[idx].name == NULL)
		return SNASH_MALLOC_ERROR;

	strcpy(p_vars->buf[idx].name, p_name);

	p_vars->buf[idx].val = malloc(strlen(p_val) + 1);
	if (p_vars->buf[idx].val == NULL)
		return SNASH_MALLOC_ERROR;

	strcpy(p_vars->buf[idx].val, p_val);

	return SNASH_NO_ERROR;
};

char *Snash_GetVar(
	Snash_Vars *p_vars,
	const char *p_name
) {
	if (p_vars == NULL || p_vars->buf == NULL)
		return NULL;


	for (usize i = 0; i < p_vars->len; ++ i)
		if (strcmp(p_vars->buf[i].name, p_name) == 0)
			return p_vars->buf[i].val;

	return NULL;
};

// Main snash functions
void Snash_Init(void) {
	Snash_vars.buf = NULL;
	Snash_vars.len = 0;

	setenv("SHELL", Snash_path, true);
	signal(SIGINT, Snash_SignalHandler);

	using_history();
	rl_set_signals();

	char *hostname = Utils_ReadFile("/etc/hostname", 64);
	for (usize i = 0; i < strlen(hostname); ++ i) {
		if (hostname[i] == ' ' || hostname[i] == '\n') {
			hostname[i] = '\0';
			break;
		};
	};
	setenv("HOSTNAME", hostname, true);

	char *user = getenv("USER");
	char *prompt = (char*)malloc(
		strlen(user) +
		strlen(hostname) + 4
	);
	strcpy(prompt, user);
	strcat(prompt, "@");
	strcat(prompt, hostname);
	strcat(prompt, "! ");

	u8 success = Snash_SetVar(&Snash_vars, "SNASHPROMPT", prompt);
	if (success != SNASH_NO_ERROR)
		Snash_InternalError(
			success,
			"Snash_SetVar(&Snash_vars, \"SNASHPROMPT\", prompt)"
		);

	free(hostname);
	free(prompt);
};

void Snash_MainLoop(void) {
	if (Snash_state == SNASH_ST_HASTOEXIT)
		return;

	Snash_state = SNASH_ST_RUNNING;
	while (Snash_state == SNASH_ST_RUNNING) {
		char *in = readline(Snash_GetVar(&Snash_vars, "SNASHPROMPT"));

		if (in == NULL) {
			printf("readline() error: returned NULL");
			continue;
		};

		add_history(in);

		if (
			strcmp(in, "exit") == 0 ||
			Snash_state == SNASH_ST_HASTOEXIT
		)
			Snash_state = SNASH_ST_NOTRUNNING;

		free(in); // readline allocates memory and
		          // we need to free it
	};
};

void Snash_Finish(void) {
	if (Snash_vars.buf != NULL) {
		for (usize i = 0; i < Snash_vars.len; ++ i) {
			free(Snash_vars.buf[i].val);
			free(Snash_vars.buf[i].name);
		};

		free(Snash_vars.buf);
	};
};

// Rest of snash
void Snash_InternalError(const u8 p_errId, const char *p_why) {
	switch (p_errId) {
		case SNASH_NO_ERROR: return;
		case SNASH_REALLOC_ERROR:
			printf("SNASH Internal realloc() error, why: %s\n", p_why);
			break;

		case SNASH_MALLOC_ERROR:
			printf("SNASH Internal malloc() error, why: %s\n", p_why);
			break;

		case SNASH_GENERIC_ERROR:
			printf("SNASH Internal generic error, why: %s\n", p_why);
	};
};

bool Snash_ReadParams(
	const int p_argc,
	const char *p_argv[]
) {
	bool exit = false;
	for (usize i = 1; i < p_argc; ++ i) {
		if (p_argv[i][0] == '-') {
			if (
				strcmp(p_argv[i], "-v") == 0 ||
				strcmp(p_argv[i], "--version") == 0
			) {
				printf(
					"Snash version %i.%i.%i\n",
					SNASH_VERSION_MAJOR,
					SNASH_VERSION_MINOR,
					SNASH_VERSION_PATCH
				);

				exit = true;
			} else if (
				strcmp(p_argv[i], "-h") == 0 ||
				strcmp(p_argv[i], "--help") == 0
			) {
				printf(
					"Usage: app [Options]\n"
					"Options:\n"
					"    -h, --help     Show the usage\n"
					"    -v  --version  Show the current version\n"
				);

				exit = true;
			} else
				goto l_error; // Not a bad usage of labels, dont complain
		};

	l_error:
		printf("Invalid option '%s'\n", p_argv[i]);
		exit = true;
	};

	return exit;
};

void Snash_SignalHandler(const int p_signal) {

};
