#ifndef __UTILS_H_HEADER_GUARD__
#define __UTILS_H_HEADER_GUARD__

#include <stdio.h> // ftell, fseek, FILE, SEEK_END,
                   // fopen, fclose, EOF, SEEK_SET

#include "types.h"
#include "error.h"
#include "str.h"

t_str utils_readfile(t_str p_name);

#endif // __UTILS_H_HEADER_GUARD__
