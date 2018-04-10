/*
 *  serializer.c
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
#include <stdio.h>

#include "mcnbt.h"
#include "util.h"

static unsigned long _lpow(long a, long b) {
    unsigned long ret = 1;
    for (long i = b; b; b >>= 1) {
        if (b & 1) {
            ret *= a;
        }
        a *= a;
    }

    return ret;
}

static char *_serialize_numerical_value(void *num, size_t bytes) {
    char *ret = NULL;
    CALLOC(ret, bytes, sizeof(char), return NULL);

    for (int i = 0; i < bytes; i++) {
        ret[bytes - i - 1] = (char)((*((long*)num) & (_lpow(2, bytes * 8) - 1)) >> (8 * i));
    }

    return ret;
}

static char *_serialize_byte(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_node_get_type(node) == MCNBT_TAG_BYTE, return NULL);
    char *ret = NULL;
    size_t ret_size = 2;
    char *name_size = NULL;
    char byte = nbt_node_get_data_byte(node);

    char *name = nbt_node_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    name_size = _serialize_numerical_value(&name_len, 2);

    ret_size += name_len;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_BYTE;
    if (name != NULL) {
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

static char *_serialize_short(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_node_get_type(node) == MCNBT_TAG_SHORT, return NULL);
    char *ret = NULL;
    size_t ret_size = 3;
    char *name_size = NULL;
    short data = nbt_node_get_data_short(node);
    char *sdata = _serialize_numerical_value(&data, 2);

    char *name = nbt_node_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    name_size = _serialize_numerical_value(&name_len, 2);

    ret_size += name_len;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_SHORT;
    if (name != NULL) {
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

static char *_serialize_int(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_node_get_type(node) == MCNBT_TAG_INT, return NULL);
    char *ret = NULL;
    size_t ret_size = 5;
    char *name_size = NULL;
    int data = nbt_node_get_data_int(node);
    char *sdata = _serialize_numerical_value(&data, 4);

    char *name = nbt_node_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    name_size = _serialize_numerical_value(&name_len, 2);

    ret_size += name_len;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_INT;
    if (name != NULL) {
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

static char *_serialize_long(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_node_get_type(node) == MCNBT_TAG_LONG, return NULL);
    char *ret = NULL;
    size_t ret_size = 9;
    char *name_size = NULL;
    long data = nbt_node_get_data_long(node);
    char *sdata = _serialize_numerical_value(&data, 8);

    char *name = nbt_node_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    name_size = _serialize_numerical_value(&name_len, 2);

    ret_size += name_len;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_LONG;
    if (name != NULL) {
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

static char *_serialize_float(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_node_get_type(node) == MCNBT_TAG_FLOAT, return NULL);
    char *ret = NULL;
    size_t ret_size = 5;
    char *name_size = NULL;
    float data = nbt_node_get_data_float(node);
    char *sdata = _serialize_numerical_value(&data, 4);

    char *name = nbt_node_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    name_size = _serialize_numerical_value(&name_len, 2);

    ret_size += name_len;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_FLOAT;
    if (name != NULL) {
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

static char *_serialize_double(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_node_get_type(node) == MCNBT_TAG_DOUBLE, return NULL);
    char *ret = NULL;
    size_t ret_size = 9;
    char *name_size = NULL;
    double data = nbt_node_get_data_double(node);
    char *sdata = _serialize_numerical_value(&data, 8);

    char *name = nbt_node_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    name_size = _serialize_numerical_value(&name_len, 2);

    ret_size += name_len;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_DOUBLE;
    if (name != NULL) {
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

static char *_serialize_byte_array(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_node_get_type(node) == MCNBT_TAG_BYTE_ARRAY, return NULL);
    char *ret = NULL;
    size_t ret_size = 5;
    char *name_size = NULL;
    char *data = nbt_node_get_data_str(node);
    size_t dlen = nbt_node_get_len(node);
    char *sdlen = _serialize_numerical_value(&dlen, 4);

    char *name = nbt_node_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    name_size = _serialize_numerical_value(&name_len, 2);

    ret_size += name_len + dlen;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_BYTE_ARRAY;
    if (name != NULL) {
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
    FREE(sdlen);

    return ret;
}

static char *_serialize_string(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_node_get_type(node) == MCNBT_TAG_STRING, return NULL);
    char *ret = NULL;
    size_t ret_size = 3;
    char *name_size = NULL;
    char *data = nbt_node_get_data_str(node);
    size_t dlen = strlen(data);
    char *sdlen = _serialize_numerical_value(&dlen, 2);

    char *name = nbt_node_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    name_size = _serialize_numerical_value(&name_len, 2);

    ret_size += name_len + dlen;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_STRING;
    if (name != NULL) {
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
    FREE(sdlen);

    return ret;
}

static char *_serialize_compound(nbt_node_t *node, size_t *len);
static char *_serialize_int_array(nbt_node_t *node, size_t *len);
static char *_serialize_long_array(nbt_node_t *node, size_t *len);

static char *_serialize_list(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_node_get_type(node) == MCNBT_TAG_LIST, return NULL);
    char *ret = NULL;
    size_t ret_size = 6;
    char *name_size = NULL;
    size_t l = nbt_node_get_len(node);
    char *sl = _serialize_numerical_value(&l, 4);
    char **tmp = NULL;
    char *t = NULL;
    nbt_node_t *tmpnode;
    size_t sizes[l];
    size_t tmplen = 0;
    size_t content_len = 0;
    int cur_pos;

    char *name = nbt_node_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    name_size = _serialize_numerical_value(&name_len, 2);

    CALLOC(tmp, l, sizeof(char *), return NULL);
    tmpnode = nbt_node_get_first_child(node);
    for (int i = 0; i < l; i++) {
        if (tmpnode == NULL) {
            fprintf(stderr, "too few nodes in list\n");
            goto free;
        }

        switch (nbt_node_get_type(tmpnode)) {
            case MCNBT_TAG_BYTE:
                t = _serialize_byte(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_SHORT:
                t = _serialize_short(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_INT:
                t = _serialize_int(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_LONG:
                t = _serialize_long(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_FLOAT:
                t = _serialize_float(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_DOUBLE:
                t = _serialize_double(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_BYTE_ARRAY:
                t = _serialize_byte_array(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_STRING:
                t = _serialize_string(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_LIST:
                t = _serialize_list(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_COMPOUND:
                t = _serialize_compound(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_INT_ARRAY:
                t = _serialize_int_array(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_LONG_ARRAY:
                t = _serialize_long_array(tmpnode, &tmplen);
                break;
            default:
                break;
        }

        *(tmp + i) = t;
        sizes[i] = tmplen;
        content_len += tmplen;
        tmpnode = nbt_node_get_next_child(tmpnode);
    }

    ret_size += name_len + content_len - l;
    CALLOC(ret, ret_size, sizeof(char), return NULL);
    ret[0] = MCNBT_TAG_LIST;
    if (name != NULL) {
        _mcnbt_memcat(ret, name_size, 2, 1);
        _mcnbt_memcat(ret, name, strlen(name), 3);
        ret[3 + strlen(name)] = nbt_node_get_list_type(node);
        _mcnbt_memcat(ret, sl, 4, 4 + (int) strlen(name));

        cur_pos = 8 + (int) strlen(name);
        for (int i = 0; i < l; i++) {
            _mcnbt_memcat(ret, (tmp[i] + 1), sizes[i] - 1, cur_pos);
            cur_pos += sizes[i] - 1;
        }
    } else {
        ret[1] = nbt_node_get_list_type(node);
        _mcnbt_memcat(ret, sl, 4, 2);

        cur_pos = 6;
        for (int i = 0; i < l; i++) {
            _mcnbt_memcat(ret, (tmp[i] + 1), sizes[i] - 1, cur_pos);
            cur_pos += sizes[i] - 1;
        }
    }

    if (len != NULL) {
        *len = ret_size;
    }

free:
    FREE(name_size);
    FREE(sl);
    for (int i = 0; i < l; i++) {
        FREE(tmp[i]);
    }
    FREE(tmp);
    return ret;
}

static char *_serialize_int_array(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_node_get_type(node) == MCNBT_TAG_INT_ARRAY, return NULL);
    char *ret = NULL;
    size_t ret_size = 5;
    char *name_size = NULL;
    int *data = nbt_node_get_data_int_array(node);
    size_t dlen = nbt_node_get_len(node);
    char *sdlen = _serialize_numerical_value(&dlen, 4);
    int pos;
    char *tmp;

    char *name = nbt_node_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    name_size = _serialize_numerical_value(&name_len, 2);

    ret_size += name_len + dlen * 4;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_INT_ARRAY;
    if (name != NULL) {
        _mcnbt_memcat(ret, name_size, 2, 1);
        _mcnbt_memcat(ret, name, strlen(name), 3);
        _mcnbt_memcat(ret, sdlen, 4, 3 + (int) strlen(name));

        pos = 7 + (int) strlen(name);
        for (int i = 0; i < dlen; i++) {
            tmp = _serialize_numerical_value(&data[i], 4);
            _mcnbt_memcat(ret, tmp, 4, pos);
            pos += 4;
            FREE(tmp);
        }
    } else {
        _mcnbt_memcat(ret, sdlen, 4, 1);

        pos = 5;
        for (int i = 0; i < dlen; i++) {
            tmp = _serialize_numerical_value(&data[i], 4);
            _mcnbt_memcat(ret, tmp, 4, pos);
            pos += 4;
            FREE(tmp);
        }
    }

    if (len != NULL) {
        *len = ret_size;
    }

    FREE(name_size);
    FREE(sdlen);

    return ret;
}

static char *_serialize_long_array(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_node_get_type(node) == MCNBT_TAG_LONG_ARRAY, return NULL);
    char *ret = NULL;
    size_t ret_size = 5;
    char *name_size = NULL;
    long *data = nbt_node_get_data_long_array(node);
    size_t dlen = nbt_node_get_len(node);
    char *sdlen = _serialize_numerical_value(&dlen, 4);
    char *tmp;
    int pos;

    char *name = nbt_node_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    name_size = _serialize_numerical_value(&name_len, 2);

    ret_size += name_len + dlen * 8;
    CALLOC(ret, ret_size, sizeof(char), return NULL);

    ret[0] = MCNBT_TAG_LONG_ARRAY;
    if (name != NULL) {
        _mcnbt_memcat(ret, name_size, 2, 1);
        _mcnbt_memcat(ret, name, strlen(name), 3);
        _mcnbt_memcat(ret, sdlen, 4, 3 + (int) strlen(name));

        pos = 7 + (int) strlen(name);
        for (int i = 0; i < dlen; i++) {
            tmp = _serialize_numerical_value(&data[i], 8);
            _mcnbt_memcat(ret, tmp, 8, pos);
            pos += 8;
            FREE(tmp);
        }
    } else {
        _mcnbt_memcat(ret, sdlen, 4, 1);

        pos = 5;
        for (int i = 0; i < dlen; i++) {
            tmp = _serialize_numerical_value(&data[i], 8);
            _mcnbt_memcat(ret, tmp, 8, pos);
            pos += 8;
            FREE(tmp);
        }
    }

    if (len != NULL) {
        *len = ret_size;
    }

    FREE(name_size);
    FREE(sdlen);

    return ret;
}

static char *_serialize_compound(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);
    ASSERT(nbt_node_get_type(node) == MCNBT_TAG_COMPOUND, return NULL);
    char *ret = NULL;
    size_t ret_size = 2;
    char *name_size = NULL;
    size_t l = nbt_node_get_len(node);
    char **tmp = NULL;
    char *t = NULL;
    nbt_node_t *tmpnode;
    size_t sizes[l];
    size_t tmplen = 0;
    size_t content_len = 0;
    int cur_pos;

    char *name = nbt_node_get_name(node);
    size_t name_len;
    if (name != NULL) {
        ret_size += 2;
        name_len = strlen(name);
    } else {
        name_len = 0;
    }

    name_size = _serialize_numerical_value(&name_len, 2);

    CALLOC(tmp, l, sizeof(char *), return NULL);
    tmpnode = nbt_node_get_first_child(node);
    for (int i = 0; i < l; i++) {
        if (tmpnode == NULL) {
            fprintf(stderr, "too few nodes in list\n");
            goto free;
        }

        switch (nbt_node_get_type(tmpnode)) {
            case MCNBT_TAG_BYTE:
                t = _serialize_byte(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_SHORT:
                t = _serialize_short(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_INT:
                t = _serialize_int(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_LONG:
                t = _serialize_long(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_FLOAT:
                t = _serialize_float(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_DOUBLE:
                t = _serialize_double(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_BYTE_ARRAY:
                t = _serialize_byte_array(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_STRING:
                t = _serialize_string(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_LIST:
                t = _serialize_list(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_COMPOUND:
                t = _serialize_compound(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_INT_ARRAY:
                t = _serialize_int_array(tmpnode, &tmplen);
                break;
            case MCNBT_TAG_LONG_ARRAY:
                t = _serialize_long_array(tmpnode, &tmplen);
                break;
            default:
                break;
        }

        *(tmp + i) = t;
        sizes[i] = tmplen;
        content_len += tmplen;
        tmpnode = nbt_node_get_next_child(tmpnode);
    }

    ret_size += name_len + content_len;
    CALLOC(ret, ret_size, sizeof(char), return NULL);
    ret[0] = MCNBT_TAG_COMPOUND;
    if (name != NULL) {
        _mcnbt_memcat(ret, name_size, 2, 1);
        _mcnbt_memcat(ret, name, strlen(name), 3);

        cur_pos = 3 + (int) strlen(name);
        for (int i = 0; i < l; i++) {
            _mcnbt_memcat(ret, tmp[i], sizes[i], cur_pos);
            cur_pos += sizes[i];
        }
    } else {
        cur_pos = 1;
        for (int i = 0; i < l; i++) {
            _mcnbt_memcat(ret, tmp[i], sizes[i], cur_pos);
            cur_pos += sizes[i];
        }
    }
    ret[cur_pos] = MCNBT_TAG_END;

    if (len != NULL) {
        *len = ret_size;
    }

    free:
    FREE(name_size);
    for (int i = 0; i < l; i++) {
        FREE(tmp[i]);
    }
    FREE(tmp);
    return ret;
}

char *nbt_node_serialize(nbt_node_t *node, size_t *len) {
    ASSERT(node != NULL, return NULL);

    if (nbt_node_get_type(node) != MCNBT_TAG_COMPOUND) {
        fprintf(stderr, "root node must be compound\n");
        return NULL;
    }

    *len = 0;
    return _serialize_compound(node, len);
}