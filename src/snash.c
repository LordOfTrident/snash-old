#include "snash.h"

// extern
u8    g_snash_exitcode = SNASH_EC_OK;
t_str g_snash_path     = STR_NULL;

// file-scoped
t_str snash_builtin[SNASH_BIC_CNT] = {
	STR("help", 4),
	STR("cd",   2), // Change directory
	STR("exit", 4)
};

t_vars g_snash_vars;
bool   g_snash_running = false;
t_str  g_snash_prompt  = STR_NULL;

void snash_init(void) {
	vars_init(&g_snash_vars);
	if (g_error.hpnd) {
		eprintf("FROM %s:%lu\n", ERR_FILE, (unsigned long)ERR_LINE);
		error();
	};

	struct sigaction sig_int_handler;
	sig_int_handler.sa_handler = SIG_IGN;
	sigemptyset(&sig_int_handler.sa_mask);
	sig_int_handler.sa_flags = 0;

	sigaction(SIGINT, &sig_int_handler, NULL);
	setenv(SNASH_VAR_SHELL, g_snash_path.buf, true);

	using_history();
	rl_set_signals();

	setenv(SNASH_VAR_PDEF, "$ ", true);
	setenv(SNASH_VAR_PSEC, COLOR_RESET"> ", true);
	setenv(
		SNASH_VAR_VERSION,
		TO_STR(SNASH_VERSION_MAJOR)"."
		TO_STR(SNASH_VERSION_MINOR)"."
		TO_STR(SNASH_VERSION_PATCH),
		true
	);
};

void snash_main_loop(void) {
	ERR_WATCH_INIT;
	g_snash_running = true;
	while (g_snash_running) {
		snash_update();

		t_str in = str(readline(g_snash_prompt.buf)); ERR_WATCH;

		if (in.buf == NULL) {
			eprintf("FROM %s:%lu\n", ERR_FILE, (unsigned long)ERR_LINE);
			ERR_SET_G_ERROR("in.buf is NULL");
			error();
		};

		add_history(in.buf);

		if (str_comp(in, str("exit")))
			g_snash_running = false;
		else {
			lex(in);
			if (g_error.hpnd) {
				eprintf("FROM %s:%lu\n", ERR_FILE, (unsigned long)ERR_LINE);
				error();
			};
			if (g_lex_error.hpnd) {
				eprintf(
					"LEXER error:\n"
					"  why: %s\n"
					"  col: %lu\n"
					"  row: %lu\n",
					g_lex_error.why,
					(unsigned long)g_lex_error.col,
					(unsigned long)g_lex_error.row
				);
				eflush();
			};
		};

		str_free(&in);
	};
};

void snash_finish(void) {
	str_free(&g_snash_path);
	vars_free(&g_snash_vars);
	if (g_error.hpnd) {
		eprintf("FROM %s:%lu\n", ERR_FILE, (unsigned long)ERR_LINE);
		error();
	};
};

static void snash_update(void) {
	snash_set_env_vars();

	// These strings are on the heap, they have to be freed!
	if (!str_comp_entire(g_snash_prompt, STR_NULL))
		str_free(&g_snash_prompt);

	g_snash_prompt = str_copy(str(getenv(SNASH_VAR_PDEF)));
	if (g_error.hpnd) {
		eprintf("FROM %s:%lu\n", ERR_FILE, (unsigned long)ERR_LINE);
		error();
	};
};

static void snash_set_env_vars(void) {
	ERR_WATCH_INIT;
	t_str hostname = utils_readfile(str("/etc/hostname")); ERR_WATCH;
	if (hostname.buf == NULL) {
		eprintf("FROM %s:%lu\n", ERR_FILE, (unsigned long)ERR_LINE);
		ERR_SET_G_ERROR("hostname.buf is NULL");
		error();
	};
	t_str curr_dir = str(getcwd(NULL, 0)); ERR_WATCH;
	if (curr_dir.buf == NULL) {
		eprintf("FROM %s:%lu\n", ERR_FILE, (unsigned long)ERR_LINE);
		ERR_SET_G_ERROR("curr_dir.buf is NULL");
		error();
	};
	// the time string can only have 8 chars and no more, hh:mm:ss
	t_str time_whl  = str_new(); str_resize(&time_whl,  8);
	t_str time_hour = str_new(); str_resize(&time_hour, 2);
	t_str time_min  = str_new(); str_resize(&time_min,  2);
	t_str time_sec  = str_new(); str_resize(&time_sec,  2);

	time_t time_raw;
	struct tm *time_info;
	time(&time_raw);
	time_info = localtime(&time_raw);

	sprintf(
		time_whl.buf,
		"%d:%d:%d",
		time_info->tm_hour,
		time_info->tm_min,
		time_info->tm_sec
	);
	time_whl.len = strlen(time_whl.buf);

	sprintf(time_hour.buf,"%d",time_info->tm_hour);
	time_hour.len = strlen(time_hour.buf);

	sprintf(time_min.buf,"%d",time_info->tm_min);
	time_min.len = strlen(time_min.buf);

	sprintf(time_sec.buf,"%d",time_info->tm_sec);
	time_sec.len = strlen(time_sec.buf);

	// Remove trailing whitespaces
	for (usize i = hostname.len - 1; i > 0; -- i) {
		char ch = hostname.buf[i];
		if (ch != ' ' && ch != '\n' && ch != '\r' && ch != '\t' && ch != '\0') {
			str_substr_into(&hostname, 0, i + 1);
			break;
		};
	};
	if (g_error.hpnd) {
		eprintf("FROM %s:%lu\n", ERR_FILE, (unsigned long)ERR_LINE);
		error();
	};

	setenv(SNASH_VAR_HOSTNAME,  hostname.buf,  true); str_free(&hostname);
	setenv(SNASH_VAR_CURR_DIR,  curr_dir.buf,  true); str_free(&curr_dir);
	setenv(SNASH_VAR_TIME,      time_whl.buf,  true); str_free(&time_whl);
	setenv(SNASH_VAR_TIME_HOUR, time_hour.buf, true); str_free(&time_hour);
	setenv(SNASH_VAR_TIME_MIN,  time_min.buf,  true); str_free(&time_min);
	setenv(SNASH_VAR_TIME_SEC,  time_sec.buf,  true); str_free(&time_sec);
};

bool snash_read_params(int p_argc, const char *p_argv[]) {
	bool start = true;
	for (usize i = 1; i < p_argc; ++ i) {
		t_str arg = str((char*)p_argv[i]);
		if (str_comp(arg, str("-v")) || str_comp(arg, str("--version"))) {
			printf("snash version %i.%i.%i\n",
				SNASH_VERSION_MAJOR,
				SNASH_VERSION_MINOR,
				SNASH_VERSION_PATCH
			);
			start = false;
		} else if (str_comp(arg, str("-h")) || str_comp(arg, str("--help"))) {
			printf(
				"Usage: snash [options]\n"
				"Options:\n"
				"    -h, --help     Show the usage\n"
				"    -v  --version  Show the current version\n"
			);
			start = false;
		} else {
			eprintf("Invalid option '%s'\nuse -h to see the usage", p_argv[i]);
			return false;
		};
	};

	return start;
};
