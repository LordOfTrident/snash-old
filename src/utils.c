#include "utils.h"

t_str utils_readfile(t_str p_name) {
	FILE *file_handle = fopen(p_name.buf, "r");

	fseek(file_handle, 0, SEEK_END);
	usize file_size = ftell(file_handle);
	fseek(file_handle, 0, SEEK_SET);

	char* file_content = (char*)malloc(file_size + 1);
	for (usize i = 0; (file_content[i] = getc(file_handle)) != EOF; ++ i);

	fclose(file_handle);
	return (t_str){file_content, file_size};
};
