//
// Created by markzz on 3/29/18.
//

#include <malloc.h>
#include <math.h>
#include "util.h"

char *base10_to_binary(int i, int bits) {
    int j;
    char *ret = (char*)malloc(sizeof(char) * bits);

    for (j = bits; j >= 0; j--) {
        *(ret+(bits-j)) = (char)(i >= 1 << j ? 1 : 0);
        i -= i >= 1 << j ? 1 << j : 0;
    }

    return ret;
}

float bit_string_to_float(const char *bits) {
    int sign;
    int exp;

    sign = bits[0] ? -1 : 1;

}