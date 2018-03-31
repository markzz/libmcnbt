/*
 *  util.c
 *
 *  Copyright (c) 2018 Mark Weiman <mark.weiman@markzz.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of version 2.1 of the GNU Lesser General
 *  Public License as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not see <http://www.gnu.org/licenses/>.
 */

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