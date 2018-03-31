//
// Created by markzz on 3/29/18.
//

#ifndef LIBMCNBT_UTIL_H
#define LIBMCNBT_UTIL_H

#define FREE(p) do { free(p); p = NULL; } while(0)

#define ASSERT(cond, action) do { if(!(cond)) { action; } } while(0)

char *base10_to_binary(int i, int bits);
char *base10_double_to_binary(double d);

float bit_string_to_float(const char *bits);

#endif //LIBMCNBT_UTIL_H
