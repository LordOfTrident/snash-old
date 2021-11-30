#include "snash.h"

t_snash_pss g_snash_pss[SNASH_PSS_CNT] = {
	{STR("\\w", 2), STR_NULL}, // Current working directory
	{STR("\\h", 2), STR_NULL}, // Hostname
	{STR("\\u", 2), STR_NULL}, // User
	{STR("\\v", 2), STR_NULL}  // Snash version
};
t_str  g_snash_path = STR_NULL;
u8     g_snash_exitcode = SNASH_EC_OK;
t_vars g_snash_vars;
t_snash_runst g_snash_runst = snash_runst_not_running;

void snash_init(void) {
	vars_init(&g_snash_vars);
	if (g_error.hpnd)
		error();

	struct sigaction sig_int_handler;

	sig_int_handler.sa_handler = snash_signal_handler;
	sigemptyset(&sig_int_handler.sa_mask);
	sig_int_handler.sa_flags = 0;

	sigaction(SIGINT, &sig_int_handler, NULL);
	setenv(SNASH_VAR_SHELL, g_snash_path.buf, true);

	using_history();
	rl_set_signals();

	setenv(
		SNASH_VAR_PDEF,
		COLOR_F_RED"["COLOR_F_YELLOW"\\u"COLOR_F_GREEN"@"
		COLOR_F_CYAN"\\h"COLOR_F_BLUE" \\w"COLOR_F_MAGENTA"]$ "COLOR_RESET,
		true
	);
	setenv(SNASH_VAR_PSEC, COLOR_RESET"> ", true);
	setenv(SNASH_VAR_VERSION, SNASH_VERSION, true);

	g_snash_pss[SNASH_PSS_V_IDX].rep = str(SNASH_VERSION);
};

void snash_main_loop(void) {
	ERR_WATCH_INIT;
	g_snash_runst = snash_runst_running;
	while (g_snash_runst == snash_runst_running) {
		snash_update_vars();

		t_str prompt = snash_get_prompt();
		if (g_error.hpnd)
			error();

		t_str in = str(readline(prompt.buf)); ERR_WATCH;
		str_free(&prompt);

		if (in.buf == NULL) {
			ERR_SET_G_ERROR("in.buf is NULL");
			error();
		};

		add_history(in.buf);

		if (str_comp(in, str("exit")))
			g_snash_runst = snash_runst_not_running;

		str_free(&in);
	};
};

void snash_finish(void) {
	str_free(&g_snash_path);
	vars_free(&g_snash_vars);
	if (g_error.hpnd)
		error();
};

void snash_update_vars(void) {
	ERR_WATCH_INIT;
	t_str hostname = utils_readfile(str("/etc/hostname")); ERR_WATCH;
	if (hostname.buf == NULL) {
		ERR_SET_G_ERROR("hostname.buf is NULL");
		error();
	};
	t_str curr_dir = str(getcwd(NULL, 0)); ERR_WATCH;
	if (curr_dir.buf == NULL) {
		ERR_SET_G_ERROR("curr_dir.buf is NULL");
		error();
	};

	for (usize i = hostname.len - 1; i > 0; -- i) {
		char ch = hostname.buf[i];
		if (ch != ' ' && ch != '\n' && ch != '\r' && ch != '\t' && ch != '\0') {
			str_substr_into(&hostname, 0, i + 1);
			break;
		};
	};
	if (g_error.hpnd)
		error();

	setenv(SNASH_VAR_HOSTNAME, hostname.buf, true);
	setenv(SNASH_VAR_CURR_DIR, curr_dir.buf, true);

	// These strings are on the heap, they have to be freed!
	str_free(&g_snash_pss[SNASH_PSS_W_IDX].rep);
	str_free(&g_snash_pss[SNASH_PSS_H_IDX].rep);
	if (g_error.hpnd)
		g_error.hpnd = false; // str_free will error if .rep is NULL

	g_snash_pss[SNASH_PSS_W_IDX].rep = curr_dir;
	g_snash_pss[SNASH_PSS_H_IDX].rep = hostname;
	g_snash_pss[SNASH_PSS_U_IDX].rep = str(getenv(SNASH_VAR_USER));
};

t_str snash_get_prompt(void) {
	t_str prompt = str_copy(str(getenv(SNASH_VAR_PDEF)));
	for (usize i = 0; i < SNASH_PSS_CNT; ++ i) {
		t_str tmp = str_replace(prompt, g_snash_pss[i].seq, g_snash_pss[i].rep);
		str_free(&prompt);
		prompt = tmp;
	};

	return prompt;
};

bool snash_read_params(int p_argc, const char *p_argv[]) {
	bool start = true;
	for (usize i = 1; i < p_argc; ++ i) {
		if (p_argv[i][0] == '-') {
			t_str arg = str((char*)p_argv[i]);

			if (str_comp(arg, str("-v")) || str_comp(arg, str("--version"))) {
				printf("snash version %s\n", SNASH_VERSION);
				start = false;
			} else if (str_comp(arg, str("-h")) || str_comp(arg, str("--help"))) {
				printf(
					"Usage: snash [options]\n"
					"Options:\n"
					"    -h, --help     Show the usage\n"
					"    -v  --version  Show the current version\n"
				);
				start = false;
			} else
				goto l_error;
		} else {
		l_error:
			eprintf("Invalid option '%s'\n", p_argv[i]);
			start = false;
		};
	};

	return start;
};

void snash_signal_handler(int p_signal) {};
