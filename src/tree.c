/*
 *  tree.c
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
#include "tree.h"
#include "util.h"

struct _nbt_node_t {
    nbt_tag_type_t type;
    union {
        char b;
        short s;
        int i;
        long l;
        float f;
        double d;
        void *str;
    } data;

    char *name;

    struct _nbt_node_t *parent;
    struct _nbt_node_t *first_child;
    struct _nbt_node_t *last_child;
    struct _nbt_node_t *next_child;
    struct _nbt_node_t *prev_child;

    /* for byte arrays and strings */
    size_t len;

    /* used only for Lists */
    nbt_tag_type_t list_type;
};

void nbt_node_free(nbt_node_t *tree) {
    nbt_node_t *item;
    ASSERT(tree != NULL, return);

    switch (tree->type) {
        case MCNBT_TAG_COMPOUND:
        case MCNBT_TAG_LIST:
            item = tree->first_child;
            while (item != NULL) {
                nbt_node_t *tmp = item->next_child;
                nbt_node_free(item);
                item = tmp;
            }
            break;
        case MCNBT_TAG_STRING:
        case MCNBT_TAG_BYTE_ARRAY:
            FREE(tree->data.str);
        default:
            break;
    }

    if (tree->name != NULL) {
        FREE(tree->name);
    }
    FREE(tree);
}

nbt_node_t *nbt_node_initialize(nbt_tag_type_t type, const char *name, void *data) {
    return nbt_node_initialize_len(type, name, data, 0);
}

nbt_node_t *nbt_node_initialize_len(nbt_tag_type_t type, const char *name, void *data, size_t data_size) {
    nbt_node_t *ret;
    MALLOC(ret, sizeof(nbt_node_t), return NULL);

    ret->type = type;

    if (name != NULL) {
        CALLOC(ret->name, strlen(name) + 1, sizeof(char), return NULL);
        strcpy(ret->name, name);
    } else {
        ret->name = NULL;
    }

    switch (type) {
        case MCNBT_TAG_BYTE:
            ret->data.b = *((char *) data);
            break;
        case MCNBT_TAG_SHORT:
            ret->data.s = *((short *) data);
            break;
        case MCNBT_TAG_INT:
            ret->data.i = *((int *) data);
            break;
        case MCNBT_TAG_LONG:
            ret->data.l = *((long *) data);
            break;
        case MCNBT_TAG_FLOAT:
            ret->data.f = *((float *) data);
            break;
        case MCNBT_TAG_DOUBLE:
            ret->data.d = *((double *) data);
            break;
        case MCNBT_TAG_BYTE_ARRAY:
            CALLOC(ret->data.str, data_size, sizeof(char), return NULL);
            memcpy(ret->data.str, (char *) data, data_size);
            ret->len = data_size;
            break;
        case MCNBT_TAG_STRING:
            CALLOC(ret->data.str, strlen(data) + 1, sizeof(char), return NULL);
            strncpy(ret->data.str, (char *) data, strlen(data) + 1);
            ret->len = strlen(ret->data.str);
            break;
        case MCNBT_TAG_LIST:
        case MCNBT_TAG_END:
        case MCNBT_TAG_COMPOUND:
        case MCNBT_TAG_INT_ARRAY:
            CALLOC(ret->data.str, data_size, sizeof(int), return NULL);
            memcpy(ret->data.str, (char *) data, data_size);
            ret->len = data_size / 4;
            break;
        case MCNBT_TAG_LONG_ARRAY:
            CALLOC(ret->data.str, data_size, sizeof(long), return NULL);
            memcpy(ret->data.str, (char *) data, data_size);
            ret->len = data_size / 8;
            break;
        default:
            break;
    }

    ret->parent = NULL;
    ret->first_child = NULL;
    ret->last_child = NULL;
    ret->next_child = NULL;
    ret->prev_child = NULL;

    return ret;
}

nbt_node_t *nbt_node_initialize_list(nbt_tag_type_t type, const char *name, void *data, nbt_tag_type_t list_type) {
    nbt_node_t *ret = nbt_node_initialize(type, name, data);
    ret->list_type = list_type;
    return ret;
}

char *nbt_node_get_name(nbt_node_t *node) {
    ASSERT(node != NULL, return NULL);
    return node->name;
}

int nbt_node_set_name(nbt_node_t *node, const char *name) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_LIST, return -1);

    FREE(node->name);
    strcpy(node->name, name);
    return 0;
}

nbt_tag_type_t nbt_node_get_type(nbt_node_t *node) {
    if (node == NULL) {
        return MCNBT_TAG_END;
    }

    return node->type;
}

char nbt_node_get_data_byte(nbt_node_t *node) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_BYTE, return -1);
    return node->data.b;
}

int nbt_node_set_data_byte(nbt_node_t *node, char data) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_BYTE, return -1);
    node->data.b = data;
    return 0;
}

double nbt_node_get_data_double(nbt_node_t *node) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_DOUBLE, return -1);
    return node->data.d;
}

int nbt_node_set_data_double(nbt_node_t *node, double data) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_DOUBLE, return -1);
    node->data.d = data;
    return 0;
}

float nbt_node_get_data_float(nbt_node_t *node) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_FLOAT, return -1);
    return node->data.f;
}

int nbt_node_set_data_float(nbt_node_t *node, float data) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_FLOAT, return -1);
    node->data.f = data;
    return 0;
}

short nbt_node_get_data_short(nbt_node_t *node) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_SHORT, return -1);
    return node->data.s;
}

int nbt_node_set_data_short(nbt_node_t *node, short data) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_SHORT, return -1);
    node->data.s = data;
    return 0;
}

long nbt_node_get_data_long(nbt_node_t *node) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_LONG, return -1);
    return node->data.l;
}

int nbt_node_set_data_long(nbt_node_t *node, long data) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_LONG, return -1);
    node->data.l = data;
    return 0;
}

int nbt_node_get_data_int(nbt_node_t *node) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_INT, return -1);
    return node->data.i;
}

int nbt_node_set_data_int(nbt_node_t *node, int data) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_INT, return -1);
    node->data.i = data;
    return 0;
}

char *nbt_node_get_data_str(nbt_node_t *node) {
    ASSERT(node != NULL, return NULL);
    ASSERT(node->type == MCNBT_TAG_STRING || node->type == MCNBT_TAG_BYTE_ARRAY, return NULL);
    return node->data.str;
}

int nbt_node_set_data_str(nbt_node_t *node, char *data) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_STRING, return -1);
    FREE(node->data.str);
    CALLOC(node->data.str, strlen(data) + 1, sizeof(char), return -1);
    strncpy(node->data.str, data, strlen(data) + 1);
    node->len = strlen(data);
    return 0;
}

int nbt_node_set_data_byte_array(nbt_node_t *node, char *data, size_t len) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_BYTE_ARRAY, return -1);
    FREE(node->data.str);
    CALLOC(node->data.str, len, sizeof(char), return -1);
    memcpy(node->data.str, data, len);
    node->len = len;
    return 0;
}

int *nbt_node_get_data_int_array(nbt_node_t *node) {
    ASSERT(node != NULL, return NULL);
    ASSERT(node->type == MCNBT_TAG_INT_ARRAY, return NULL);
    return node->data.str;
}

int nbt_node_set_data_int_array(nbt_node_t *node, int *data, size_t len) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_BYTE_ARRAY, return -1);
    FREE(node->data.str);
    CALLOC(node->data.str, len, sizeof(int), return -1);
    memcpy(node->data.str, data, len * sizeof(int));
    node->len = len;
    return 0;
}

long *nbt_node_get_data_long_array(nbt_node_t *node) {
    ASSERT(node != NULL, return NULL);
    ASSERT(node->type == MCNBT_TAG_LONG_ARRAY, return NULL);
    return node->data.str;
}

int nbt_node_set_data_long_array(nbt_node_t *node, long *data, size_t len) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_BYTE_ARRAY, return -1);
    FREE(node->data.str);
    CALLOC(node->data.str, len, sizeof(long), return -1);
    memcpy(node->data.str, data, len * sizeof(long));
    node->len = len;
    return 0;
}

nbt_tag_type_t nbt_node_get_list_type(nbt_node_t *node) {
    ASSERT(node != NULL, return MCNBT_TAG_END);
    ASSERT(node->type == MCNBT_TAG_LIST, return MCNBT_TAG_END);
    return node->list_type;
}

nbt_node_t *nbt_node_get_first_child(nbt_node_t *node) {
    ASSERT(node != NULL, return NULL);
    return node->first_child;
}

nbt_node_t *nbt_node_get_last_child(nbt_node_t *node) {
    ASSERT(node != NULL, return NULL);
    return node->last_child;
}

nbt_node_t *nbt_node_get_parent(nbt_node_t *node) {
    ASSERT(node != NULL, return NULL);
    return node->parent;
}

nbt_node_t *nbt_node_get_next_child(nbt_node_t *node) {
    ASSERT(node != NULL, return NULL);
    return node->next_child;
}

nbt_node_t *nbt_node_get_prev_child(nbt_node_t *node) {
    ASSERT(node != NULL, return NULL);
    return node->prev_child;
}

static void _strip_name(nbt_node_t *node) {
    if (node->name != NULL) {
        FREE(node->name);
        node->name = NULL;
    }
}

static void _add_name(nbt_node_t *node) {
    if (node->name == NULL) {
        CALLOC(node->name, 1, sizeof(char), return);
    }
}

int nbt_node_append_child(nbt_node_t *parent, nbt_node_t *child) {
    ASSERT(parent != NULL, return -1);
    ASSERT(child != NULL, return -1);
    ASSERT(parent->type == MCNBT_TAG_COMPOUND || parent->type == MCNBT_TAG_LIST, return -1);

    if (parent->type == MCNBT_TAG_LIST) {
        _strip_name(child);
    }

    if (parent->type == MCNBT_TAG_COMPOUND) {
        _add_name(child);
    }

    if (parent->first_child == NULL) {
        parent->first_child = child;
        parent->last_child = child;
        child->next_child = NULL;
        child->prev_child = NULL;
    } else {
        nbt_node_t *c = parent->last_child;
        c->next_child = child;
        child->prev_child = c;
        parent->last_child = child;
    }

    child->parent = parent;

    return 0;
}

int nbt_node_prepend_child(nbt_node_t *parent, nbt_node_t *child) {
    ASSERT(parent != NULL, return -1);
    ASSERT(child != NULL, return -1);
    ASSERT(parent->type == MCNBT_TAG_COMPOUND || parent->type == MCNBT_TAG_LIST, return -1);

    if (parent->type == MCNBT_TAG_LIST) {
        _strip_name(child);
    }

    if (parent->first_child == NULL) {
        parent->first_child = child;
        parent->last_child = child;
        child->next_child = NULL;
        child->prev_child = NULL;
    } else {
        nbt_node_t *c = parent->first_child;
        c->prev_child = child;
        child->next_child = c;
        parent->first_child = child;
    }

    child->parent = parent;

    return 0;
}


int nbt_node_insert_after(nbt_node_t *left, nbt_node_t *right) {
    ASSERT(left != NULL, return -1);
    ASSERT(right != NULL, return -1);
    ASSERT(left->parent != NULL, return -1);

    if (left->parent->type == MCNBT_TAG_LIST) {
        _strip_name(right);
    }

    nbt_node_t *parent = left->parent;
    if (parent->first_child == parent->last_child) {
        parent->last_child = right;
        right->prev_child = left;
        left->next_child = right;
        right->parent = parent;
    } else {
        nbt_node_t *next = left->next_child;

        next->prev_child = right;
        right->next_child = next;

        left->next_child = right;
        right->prev_child = left;

        right->parent = parent;
    }

    return 0;
}

int nbt_node_insert_before(nbt_node_t *right, nbt_node_t *left) {
    ASSERT(left != NULL, return -1);
    ASSERT(right != NULL, return -1);
    ASSERT(right->parent != NULL, return -1);

    if (right->parent->type == MCNBT_TAG_LIST) {
        _strip_name(left);
    }

    nbt_node_t *parent = right->parent;
    if (parent->first_child == parent->last_child) {
        parent->first_child = left;
        right->prev_child = left;
        left->next_child = right;
        left->parent = parent;
    } else {
        nbt_node_t *prev = right->prev_child;

        prev->next_child = left;
        left->prev_child = prev;

        right->prev_child = left;
        left->next_child = right;

        left->parent = parent;
    }

    return 0;
}

int nbt_node_unlink(nbt_node_t *node) {
    ASSERT(node != NULL, return -1);
    nbt_node_t *parent = node->parent;

    if (node->prev_child != NULL) {
        node->prev_child->next_child = node->next_child;
    }

    if (node->next_child != NULL) {
        node->next_child->prev_child = node->prev_child;
    }

    if (parent != NULL) {
        if (parent->first_child == node) {
            parent->first_child = node->next_child;
        }

        if (parent->last_child == node) {
            parent->last_child = node->prev_child;
        }
    }

    node->next_child = NULL;
    node->prev_child = NULL;
    node->parent = NULL;
    return 0;
}

int nbt_node_replace(nbt_node_t *old, nbt_node_t *new) {
    if (nbt_node_insert_before(old, new) != 0) {
        return -1;
    }

    nbt_node_unlink(old);
    return 0;
}

size_t nbt_node_get_len(nbt_node_t *node) {
    ASSERT(node != NULL, return -1);
    size_t ret = 0;

    switch (node->type) {
        case MCNBT_TAG_COMPOUND:
        case MCNBT_TAG_LIST:
            for (nbt_node_t *n = node->first_child; n; n = n->next_child) {
                ret++;
            }
            return ret;
        case MCNBT_TAG_STRING:
        case MCNBT_TAG_BYTE_ARRAY:
        default:
            return node->len;
    }
}

int nbt_node_set_len(nbt_node_t *node, size_t len) {
    ASSERT(node != NULL, return -1);
    ASSERT(node->type == MCNBT_TAG_BYTE_ARRAY || node->type == MCNBT_TAG_STRING, return -1);
    node->len = len;
    return 0;
}
