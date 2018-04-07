/*
 *  parser.h
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
#include "tree.h"
#include "mcnbt.h"
#include "util.h"

nbt_node_t *_nbt_parse_byte_array(void *data, size_t size, int *pos, int in_list) {
    nbt_node_t *ret;
    char *name = NULL;
    size_t name_len;
    char *stor;
    int s = 0;

    if (in_list == 0) {
        name_len = (size_t)((char *) data)[*pos] << 8;
        *pos += 1;
        name_len += (size_t)((char *) data)[*pos];
        *pos += 1;

        name = (char *) malloc((name_len + 1) * sizeof(char));
        for (int i = *pos; i < *pos + name_len; i++) {
            *(name + i - *pos) = ((char *) data)[i];
        }
        *(name + name_len) = '\0';
        *pos += name_len;
    }

    stor = (char *) malloc(sizeof(char) * 4);
    for (int i = *pos; i < *pos + 4; i++) {
        *(stor + i - *pos) = ((char *) data)[i];
    }
    *pos += 4;

    s = (*(stor) & 255) << 24;
    s += (*(stor + 1) & 255) << 16;
    s += (*(stor + 2) & 255) << 8;
    s += (*(stor + 3) & 255);
    FREE(stor);

    stor = (char *) malloc(sizeof(char) * s);
    for (int i = *pos; i < *pos + s; i++) {
        *(stor + i - *pos) = ((char *) data)[i];
    }
    *pos += s;

    ret = nbt_node_initialize_len(MCNBT_TAG_BYTE_ARRAY, name, stor, (size_t) s);
    nbt_node_set_len(ret, (size_t) s);
    FREE(name);
    FREE(stor);
    return ret;
}

nbt_node_t *_nbt_parse_compound(void *data, size_t size, int *pos, int in_list) {
    nbt_node_t *ret;
    char *name = NULL;
    size_t name_len;

    if (in_list == 0) {
        name_len = (size_t)((char *) data)[*pos] << 8;
        *pos += 1;
        name_len += (size_t)((char *) data)[*pos];
        *pos += 1;

        name = (char *) malloc((name_len + 1) * sizeof(char));
        for (int i = *pos; i < *pos + name_len; i++) {
            *(name + i - *pos) = ((char *) data)[i];
        }
        *(name + name_len) = '\0';
        *pos += name_len - 1;
    }

    ret = nbt_node_initialize(MCNBT_TAG_COMPOUND, name, NULL);
    FREE(name);
    return ret;
}

nbt_node_t *_nbt_parse_num(void *data, size_t size, int *pos, nbt_tag_type_t type, int in_list) {
    nbt_node_t *ret;
    char *name = NULL;
    char *stor;
    size_t name_len = 0;
    size_t data_len;
    void *edata;

    if (in_list == 0) {
        name_len = (size_t)((char *) data)[*pos] << 8;
        *pos += 1;
        name_len += (size_t)((char *) data)[*pos];
        *pos += 1;

        name = (char *) malloc((name_len + 1) * sizeof(char));
        for (int i = *pos; i < *pos + name_len; i++) {
            *(name + i - *pos) = ((char *) data)[i];
        }
        *(name + name_len) = '\0';
        *pos += name_len;
    }

    switch (type) {
        case MCNBT_TAG_BYTE:
            data_len = 1;
            break;
        case MCNBT_TAG_SHORT:
            data_len = 2;
            break;
        case MCNBT_TAG_INT:
        case MCNBT_TAG_FLOAT:
            data_len = 4;
            break;
        case MCNBT_TAG_LONG:
        case MCNBT_TAG_DOUBLE:
            data_len = 8;
            break;
        default:
            data_len = 0;
            break;
    }

    stor = (char *) malloc(sizeof(char) * data_len);
    for (int i = *pos; i < *pos + data_len; i++) {
        *(stor + i - *pos) = ((char *) data)[i];
    }
    *pos += data_len - 1;

    if (type == MCNBT_TAG_BYTE) {
        edata = malloc(sizeof(char));
        *((char *) edata) = stor[0];
    } else if (type == MCNBT_TAG_SHORT) {
        edata = malloc(sizeof(short));
        *((short *) edata) = (short) (((short) stor[0] & 255) << 8);
        *((short *) edata) += (short) stor[1] & 255;
    } else if (type == MCNBT_TAG_INT) {
        edata = malloc(sizeof(int));
        *((int *) edata) = ((int) stor[0] & 255) << 24;
        *((int *) edata) += ((int) stor[1] & 255) << 16;
        *((int *) edata) += ((int) stor[2] & 255) << 8;
        *((int *) edata) += (int) stor[3] & 255;
    } else if (type == MCNBT_TAG_LONG) {
        edata = malloc(sizeof(long));
        *((long *) edata) = ((long) stor[0] & 255) << 56;
        *((long *) edata) += ((long) stor[1] & 255) << 48;
        *((long *) edata) += ((long) stor[2] & 255) << 40;
        *((long *) edata) += ((long) stor[3] & 255) << 32;
        *((long *) edata) += ((long) stor[4] & 255) << 24;
        *((long *) edata) += ((long) stor[5] & 255) << 16;
        *((long *) edata) += ((long) stor[6] & 255) << 8;
        *((long *) edata) += (long) stor[7] & 255;
    } else if (type == MCNBT_TAG_FLOAT) {
        edata = malloc(sizeof(int));
        *((int *) edata) = ((int) stor[0] & 255) << 24;
        *((int *) edata) += ((int) stor[1] & 255) << 16;
        *((int *) edata) += ((int) stor[2] & 255) << 8;
        *((int *) edata) += (int) stor[3] & 255;
    } else if (type == MCNBT_TAG_DOUBLE) {
        edata = malloc(sizeof(long));
        *((long *) edata) = ((long) stor[0] & 255) << 56;
        *((long *) edata) += ((long) stor[1] & 255) << 48;
        *((long *) edata) += ((long) stor[2] & 255) << 40;
        *((long *) edata) += ((long) stor[3] & 255) << 32;
        *((long *) edata) += ((long) stor[4] & 255) << 24;
        *((long *) edata) += ((long) stor[5] & 255) << 16;
        *((long *) edata) += ((long) stor[6] & 255) << 8;
        *((long *) edata) += (long) stor[7] & 255;
    } else {
        edata = NULL;
    }

    FREE(stor);

    ret = nbt_node_initialize(type, name, edata);
    FREE(edata);
    FREE(name);
    return ret;
}

nbt_node_t *_nbt_parse_string(void *, size_t, int *, int);

nbt_node_t *_nbt_parse_list(void *data, size_t size, int *pos, int in_list) {
    nbt_node_t *ret;
    char *name = NULL;
    size_t name_len;
    char *stor;
    int num;
    nbt_tag_type_t list_type;
    nbt_node_t *child;

    if (in_list == 0) {
        name_len = (size_t)((char *) data)[*pos] << 8;
        *pos += 1;
        name_len += (size_t)((char *) data)[*pos];
        *pos += 1;

        name = (char *) malloc((name_len + 1) * sizeof(char));
        for (int i = *pos; i < *pos + name_len; i++) {
            *(name + i - *pos) = ((char *) data)[i];
        }
        *(name + name_len) = '\0';
        *pos += name_len;
    }

    list_type = (nbt_tag_type_t)((char *) data)[*pos];
    *pos += 1;

    stor = (char *) malloc(sizeof(char) * 4);
    for (int i = *pos; i < *pos + 4; i++) {
        *(stor + i - *pos) = ((char *) data)[i];
    }
    *pos += 4;

    num = (*(stor) & 255) << 24;
    num += (*(stor + 1) & 255) << 16;
    num += (*(stor + 2) & 255) << 8;
    num += (*(stor + 3) & 255);
    FREE(stor);

    ret = nbt_node_initialize_list(MCNBT_TAG_LIST, name, NULL, list_type);

    for (int i = 0; i < num; i++) {
        if (list_type == MCNBT_TAG_BYTE || list_type == MCNBT_TAG_SHORT || list_type == MCNBT_TAG_INT ||
                list_type == MCNBT_TAG_LONG || list_type == MCNBT_TAG_FLOAT || list_type == MCNBT_TAG_DOUBLE) {
            child = _nbt_parse_num(data, size, pos, list_type, 1);
            nbt_node_append_child(ret, child);
            (*pos)++;
        } else if (list_type == MCNBT_TAG_STRING) {
            child = _nbt_parse_string(data, size, pos, 1);
            nbt_node_append_child(ret, child);
            (*pos)++;
        } else if (list_type == MCNBT_TAG_BYTE_ARRAY) {
            child = _nbt_parse_byte_array(data, size, pos, 1);
            nbt_node_append_child(ret, child);
            (*pos)++;
        } else if (list_type == MCNBT_TAG_COMPOUND) {
            child = _nbt_parse_compound(data, size, pos, 1);
            child = _nbt_parse(data, size, child, pos);
            nbt_node_append_child(ret, child);
            (*pos)++;
        }
    }

    (*pos)--;
    FREE(name);
    return ret;
}

nbt_node_t *_nbt_parse_string(void *data, size_t size, int *pos, int in_list) {
    nbt_node_t *ret;
    char *name = NULL;
    size_t name_len;
    char *stor;
    int s = 0;

    if (in_list == 0) {
        name_len = (size_t)((char *) data)[*pos] << 8;
        *pos += 1;
        name_len += (size_t)((char *) data)[*pos];
        *pos += 1;

        name = (char *) malloc((name_len + 1) * sizeof(char));
        for (int i = *pos; i < *pos + name_len; i++) {
            *(name + i - *pos) = ((char *) data)[i];
        }
        *(name + name_len) = '\0';
        *pos += name_len;
    }

    stor = (char *) malloc(sizeof(char) * 2);
    for (int i = *pos; i < *pos + 2; i++) {
        *(stor + i - *pos) = ((char *) data)[i];
    }
    *pos += 2;

    s = (*(stor) & 255) << 8;
    s += *(stor + 1) & 255;
    FREE(stor);

    stor = (char *) malloc(sizeof(char) * (s + 1));
    for (int i = *pos; i < *pos + s; i++) {
        *(stor + i - *pos) = ((char *) data)[i];
    }
    *(stor + s) = '\0';
    *pos += s - 1;

    ret = nbt_node_initialize_len(MCNBT_TAG_STRING, name, stor, strlen(stor) + 1);
    FREE(stor);
    FREE(name);
    return ret;
}

nbt_node_t *_nbt_parse(void *data, size_t size, nbt_node_t *root, int *pos) {
    nbt_node_t *ret;
    nbt_node_t *current_parent = NULL;
    nbt_node_t *tmp;
    nbt_tag_type_t type;
    int islist;
    int i;

    if (root == NULL) {
        ret = NULL;
    } else {
        ret = root;
        current_parent = root;
    }

    if (pos == NULL) {
        i = 0;
    } else {
        i = *pos;
    }

    for (i; i < size; i++) {
        type = (nbt_tag_type_t)((char *) data)[i];
        i++;
        if (nbt_node_get_type(current_parent) == MCNBT_TAG_LIST) {
            islist = 1;
        } else {
            islist = 0;
        }

        switch (type) {
            case MCNBT_TAG_BYTE:
            case MCNBT_TAG_SHORT:
            case MCNBT_TAG_INT:
            case MCNBT_TAG_LONG:
            case MCNBT_TAG_FLOAT:
            case MCNBT_TAG_DOUBLE:
                tmp = _nbt_parse_num(data, size, &i, type, islist);
                nbt_node_append_child(current_parent, tmp);
                break;
            case MCNBT_TAG_COMPOUND:
                if (ret == NULL) {
                    ret = _nbt_parse_compound(data, size, &i, islist);
                    current_parent = ret;
                } else {
                    tmp = _nbt_parse_compound(data, size, &i, islist);
                    nbt_node_append_child(current_parent, tmp);
                    current_parent = tmp;
                }
                break;
            case MCNBT_TAG_LIST:
                tmp = _nbt_parse_list(data, size, &i, islist);
                nbt_node_append_child(current_parent, tmp);
                break;
            case MCNBT_TAG_BYTE_ARRAY:
                tmp = _nbt_parse_byte_array(data, size, &i, islist);
                nbt_node_append_child(current_parent, tmp);
                i--;
                break;
            case MCNBT_TAG_STRING:
                tmp = _nbt_parse_string(data, size, &i, islist);
                nbt_node_append_child(current_parent, tmp);
                break;
            case MCNBT_TAG_END:
                current_parent = nbt_node_get_parent(current_parent);
                i--;
            default:
                break;
        }

        if (current_parent == NULL) {
            break;
        }
    }

    if (pos != NULL) {
        *pos = i;
    }

    return ret;
}