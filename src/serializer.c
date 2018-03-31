/*
 *  serializer.h
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

#include <stdlib.h>
#include <string.h>

#include "mcnbt.h"
#include "util.h"

unsigned long _lpow(long a, long b) {
    unsigned long long ret = 1;
    for (long i = b; b; b >>= 1) {
        if (b & 1) {
            ret *= a;
        }
        a *= a;
    }

    return ret;
}

char *_serialize_numerical_value(void *num, size_t bytes) {
    char *ret = calloc(bytes + 1, sizeof(char));

    for (int i = 0; i < bytes; i++) {
        ret[bytes - i - 1] = (unsigned char)((*((long*)num) & (_lpow(2, bytes * 8) - 1)) >> (8 * i));
    }

    return ret;
}

char *_serialize_byte(nbt_node_t *node) {
    ASSERT(node != NULL, return NULL);

    char *name = nbt_tree_get_name(node);
    size_t name_len;
    if (name != NULL) {
        name_len = strlen(name);
    } else {
        name_len = 0;
    }


}