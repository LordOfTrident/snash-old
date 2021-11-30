#include "snash.h"

int main(int argc, const char *argv[]) {
	if (!snash_read_params(argc, argv))
		return SNASH_EC_OK;

	g_snash_path = str_copy(str((char*)argv[0]));
	snash_init();
	snash_main_loop();
	snash_finish();

	return g_snash_exitcode;
};
