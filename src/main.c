#include "snash.h"

int main(const int argc, const char *argv[]) {
	if (Snash_ReadParams(argc, argv) == true)
		return SNASH_NO_ERROR;

	Snash_path = (char*)argv[0];

	Snash_Init();
	Snash_MainLoop();
	Snash_Finish();

	return Snash_exitcode;
};
