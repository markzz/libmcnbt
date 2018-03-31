//
// Created by markzz on 3/30/18.
//

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