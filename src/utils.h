#ifndef __UTILS_H_HEADER_GUARD__
#define __UTILS_H_HEADER_GUARD__

#include "components.h"
#include "types.h"

char *Utils_StrReplace(
	char *p_str,
	char *p_toRep,
	char *p_rep
);
char *Utils_ReadFile(const char *p_name, const usize p_chunkSize);

#endif // __UTILS_H_HEADER_GUARD__
