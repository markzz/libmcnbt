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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "util.h"

void _mcnbt_alloc_fail(size_t size) {
    fprintf(stderr, "allocation failure: could not allocate %zu bytes\n", size);
}

/** Concatenates memory to a certain position
 * @param dest Destination pointer
 * @param src Source pointer
 * @param n Number of bytes to copy
 * @param pos Starting position in dest
 * @return Destination pointer
 */
void *_mcnbt_memcat(void *dest, const void *src, size_t n, int pos) {
    void *start_ptr = dest + pos;
    memcpy(start_ptr, src, n);
    return dest;
}