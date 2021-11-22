#include "utils.h"

char *Utils_StrReplace(
	char *p_str,
	char *p_toRep,
	char *p_rep
) {
	if (p_str == NULL || p_toRep == NULL)
		return NULL;

	if (p_rep == NULL)
		p_rep = "";

	usize toRepLen = strlen(p_toRep);
	usize repLen   = strlen(p_rep);
	usize count;

	// Count the number of times p_toRep occurs in p_str
	char *ins = p_str;
	for (
		count = 0;
		(ins = strstr(ins, p_toRep)) != NULL;
		++ count, ins += repLen
	);

	char *result = (char*)malloc(
		strlen(p_str) +
		(repLen - toRepLen) *
		count + 1
	);

	// Dont forget to make sure malloc didnt fail
	if (result == NULL)
		return NULL;

	ins = p_str;
	char *tmp = result;
	while (count --) {
		ins = strstr(p_str, p_toRep);
		usize pos = ins - p_str;

		tmp = strncpy(tmp, p_str, pos) + pos;
		tmp = strcpy(tmp, p_rep) + repLen;
		p_str += pos + repLen;
	};

	strcat(tmp, p_str);

	return result;
};

char *Utils_ReadFile(
	const char *p_name,
	const usize p_chunkSize
) {
	FILE *file;
	char *line = NULL;
	usize bufLen = p_chunkSize;
	char *buf = (char*)calloc(bufLen, sizeof(char));
	usize len = 0;
	usize lineReadLen;
	usize readLen = bufLen;

	file = fopen(p_name, "r");
	if (file == NULL)
		return NULL;

	while (
		(lineReadLen = (usize)getline(
			&line, &len, file
		)) != -1
	) {
		readLen += lineReadLen;
		if (readLen > bufLen) {
			bufLen = readLen;
			void *tmp = realloc(buf, bufLen + 1);
			if (tmp == NULL) {
				free(buf);
				return NULL;
			};

			buf = (char*)tmp;
		};

		strncat(buf, line, lineReadLen);
	};

	fclose(file);
	if (line != NULL)
		free(line);

	return buf;
};
