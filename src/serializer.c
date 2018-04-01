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
    char *ret = NULL;
    CALLOC(ret, bytes, sizeof(char), return NULL);

    for (int i = 0; i < bytes; i++) {
        ret[bytes - i - 1] = (char)((*((long*)num) & (_lpow(2, bytes * 8) - 1)) >> (8 * i));
    }

    return ret;
}

char *_serialize_byte(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_tree_get_type(node) == MCNBT_TAG_BYTE, return NULL);
    char *ret = NULL;
    size_t ret_size = 2;
    char *name_size = NULL;
    char byte = nbt_tree_get_data_byte(node);

    char *name = nbt_tree_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    if (name_len != 0) {
        name_size = _serialize_numerical_value(&name_len, 2);
    }

    ret_size += name_len;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_BYTE;
    if (name_len > 0) {
        _mcnbt_memcat(ret, name_size, 2, 1);
        _mcnbt_memcat(ret, name, strlen(name), 3);
        _mcnbt_memcat(ret, &byte, 1, 3 + (int) strlen(name));
    } else {
        _mcnbt_memcat(ret, &byte, 1, 1);
    }

    if (len != NULL) {
        *len = ret_size;
    }

    FREE(name_size);

    return ret;
}

char *_serialize_short(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_tree_get_type(node) == MCNBT_TAG_SHORT, return NULL);
    char *ret = NULL;
    size_t ret_size = 3;
    char *name_size = NULL;
    short data = nbt_tree_get_data_short(node);
    char *sdata = _serialize_numerical_value(&data, 2);

    char *name = nbt_tree_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    if (name_len != 0) {
        name_size = _serialize_numerical_value(&name_len, 2);
    }

    ret_size += name_len;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_SHORT;
    if (name_len > 0) {
        _mcnbt_memcat(ret, name_size, 2, 1);
        _mcnbt_memcat(ret, name, strlen(name), 3);
        _mcnbt_memcat(ret, sdata, 2, 3 + (int) strlen(name));
    } else {
        _mcnbt_memcat(ret, sdata, 2, 1);
    }

    if (len != NULL) {
        *len = ret_size;
    }

    FREE(name_size);
    FREE(sdata);

    return ret;
}

char *_serialize_int(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_tree_get_type(node) == MCNBT_TAG_INT, return NULL);
    char *ret = NULL;
    size_t ret_size = 5;
    char *name_size = NULL;
    int data = nbt_tree_get_data_int(node);
    char *sdata = _serialize_numerical_value(&data, 4);

    char *name = nbt_tree_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    if (name_len != 0) {
        name_size = _serialize_numerical_value(&name_len, 2);
    }

    ret_size += name_len;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_INT;
    if (name_len > 0) {
        _mcnbt_memcat(ret, name_size, 2, 1);
        _mcnbt_memcat(ret, name, strlen(name), 3);
        _mcnbt_memcat(ret, sdata, 4, 3 + (int) strlen(name));
    } else {
        _mcnbt_memcat(ret, sdata, 4, 1);
    }

    if (len != NULL) {
        *len = ret_size;
    }

    FREE(name_size);
    FREE(sdata);

    return ret;
}

char *_serialize_long(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_tree_get_type(node) == MCNBT_TAG_LONG, return NULL);
    char *ret = NULL;
    size_t ret_size = 9;
    char *name_size = NULL;
    long data = nbt_tree_get_data_long(node);
    char *sdata = _serialize_numerical_value(&data, 8);

    char *name = nbt_tree_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    if (name_len != 0) {
        name_size = _serialize_numerical_value(&name_len, 2);
    }

    ret_size += name_len;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_LONG;
    if (name_len > 0) {
        _mcnbt_memcat(ret, name_size, 2, 1);
        _mcnbt_memcat(ret, name, strlen(name), 3);
        _mcnbt_memcat(ret, sdata, 8, 3 + (int) strlen(name));
    } else {
        _mcnbt_memcat(ret, sdata, 8, 1);
    }

    if (len != NULL) {
        *len = ret_size;
    }

    FREE(name_size);
    FREE(sdata);

    return ret;
}

char *_serialize_float(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_tree_get_type(node) == MCNBT_TAG_FLOAT, return NULL);
    char *ret = NULL;
    size_t ret_size = 5;
    char *name_size = NULL;
    float data = nbt_tree_get_data_float(node);
    char *sdata = _serialize_numerical_value(&data, 4);

    char *name = nbt_tree_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    if (name_len != 0) {
        name_size = _serialize_numerical_value(&name_len, 2);
    }

    ret_size += name_len;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_FLOAT;
    if (name_len > 0) {
        _mcnbt_memcat(ret, name_size, 2, 1);
        _mcnbt_memcat(ret, name, strlen(name), 3);
        _mcnbt_memcat(ret, sdata, 4, 3 + (int) strlen(name));
    } else {
        _mcnbt_memcat(ret, sdata, 4, 1);
    }

    if (len != NULL) {
        *len = ret_size;
    }

    FREE(name_size);
    FREE(sdata);

    return ret;
}

char *_serialize_double(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_tree_get_type(node) == MCNBT_TAG_DOUBLE, return NULL);
    char *ret = NULL;
    size_t ret_size = 9;
    char *name_size = NULL;
    double data = nbt_tree_get_data_double(node);
    char *sdata = _serialize_numerical_value(&data, 8);

    char *name = nbt_tree_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    if (name_len != 0) {
        name_size = _serialize_numerical_value(&name_len, 2);
    }

    ret_size += name_len;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_DOUBLE;
    if (name_len > 0) {
        _mcnbt_memcat(ret, name_size, 2, 1);
        _mcnbt_memcat(ret, name, strlen(name), 3);
        _mcnbt_memcat(ret, sdata, 8, 3 + (int) strlen(name));
    } else {
        _mcnbt_memcat(ret, sdata, 8, 1);
    }

    if (len != NULL) {
        *len = ret_size;
    }

    FREE(name_size);
    FREE(sdata);

    return ret;
}

char *_serialize_byte_array(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_tree_get_type(node) == MCNBT_TAG_BYTE_ARRAY, return NULL);
    char *ret = NULL;
    size_t ret_size = 5;
    char *name_size = NULL;
    char *data = nbt_tree_get_data_str(node);
    size_t dlen = nbt_tree_get_len(node);
    char *sdlen = _serialize_numerical_value(&dlen, 4);

    char *name = nbt_tree_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    if (name_len != 0) {
        name_size = _serialize_numerical_value(&name_len, 2);
    }

    ret_size += name_len + dlen;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_BYTE_ARRAY;
    if (name_len > 0) {
        _mcnbt_memcat(ret, name_size, 2, 1);
        _mcnbt_memcat(ret, name, strlen(name), 3);
        _mcnbt_memcat(ret, sdlen, 4, 3 + (int) strlen(name));
        _mcnbt_memcat(ret, data, dlen, 7 + (int) strlen(name));
    } else {
        _mcnbt_memcat(ret, sdlen, 4, 1);
        _mcnbt_memcat(ret, data, dlen, 5);
    }

    if (len != NULL) {
        *len = ret_size;
    }

    FREE(name_size);
    FREE(data);
    FREE(sdlen);

    return ret;
}

char *_serialize_string(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_tree_get_type(node) == MCNBT_TAG_STRING, return NULL);
    char *ret = NULL;
    size_t ret_size = 3;
    char *name_size = NULL;
    char *data = nbt_tree_get_data_str(node);
    size_t dlen = strlen(data);
    char *sdlen = _serialize_numerical_value(&dlen, 2);

    char *name = nbt_tree_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    if (name_len != 0) {
        name_size = _serialize_numerical_value(&name_len, 2);
    }

    ret_size += name_len + dlen;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_STRING;
    if (name_len > 0) {
        _mcnbt_memcat(ret, name_size, 2, 1);
        _mcnbt_memcat(ret, name, strlen(name), 3);
        _mcnbt_memcat(ret, sdlen, 2, 3 + (int) strlen(name));
        _mcnbt_memcat(ret, data, dlen, 5 + (int) strlen(name));
    } else {
        _mcnbt_memcat(ret, sdlen, 2, 1);
        _mcnbt_memcat(ret, data, dlen, 3);
    }

    if (len != NULL) {
        *len = ret_size;
    }

    FREE(name_size);
    FREE(data);
    FREE(sdlen);

    return ret;
}

char *_serialize_list(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_tree_get_type(node) == MCNBT_TAG_LIST, return NULL);
    char *ret = NULL;
    size_t ret_size = 2;
    char *name_size = NULL;
    size_t l = nbt_tree_get_len(node);
    char *sl = _serialize_numerical_value(&l, 4);

    char *name = nbt_tree_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    if (name_len != 0) {
        name_size = _serialize_numerical_value(&name_len, 2);
    }

    for (int i = 0; i < l; i++) {

    }
}