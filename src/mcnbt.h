/*
 *  mcnbt.h
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

#ifndef LIBMCNBT_MCNBT_H
#define LIBMCNBT_MCNBT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef enum _tag_type_t {
    MCNBT_TAG_END,
    MCNBT_TAG_BYTE,
    MCNBT_TAG_SHORT,
    MCNBT_TAG_INT,
    MCNBT_TAG_LONG,
    MCNBT_TAG_FLOAT,
    MCNBT_TAG_DOUBLE,
    MCNBT_TAG_BYTE_ARRAY,
    MCNBT_TAG_STRING,
    MCNBT_TAG_LIST,
    MCNBT_TAG_COMPOUND,
    MCNBT_TAG_INT_ARRAY,  /* unimplemented */
    MCNBT_TAG_LONG_ARRAY, /* unimplemented */
} nbt_tag_type_t;

typedef struct _nbt_node_t nbt_node_t;

nbt_node_t *nbt_initialize_from_file(const char *filename);

nbt_node_t *nbt_initialize(void *data, size_t size);

nbt_node_t *nbt_tree_get_next(nbt_node_t *node);

nbt_node_t *nbt_tree_get_prev(nbt_node_t *node);

nbt_node_t *nbt_tree_get_root(nbt_node_t *node);

void nbt_tree_free(nbt_node_t *tree);

nbt_node_t *nbt_tree_initialize(nbt_tag_type_t type, const char *name, void *data);

nbt_node_t *nbt_tree_initialize_len(nbt_tag_type_t type, const char *name, void *data, size_t data_size);

nbt_node_t *nbt_tree_initialize_list(nbt_tag_type_t type, const char *name, void *data, nbt_tag_type_t list_type);

char *nbt_tree_get_name(nbt_node_t *node);

int nbt_tree_set_name(nbt_node_t *node, const char *name);

nbt_tag_type_t nbt_tree_get_type(nbt_node_t *node);

char nbt_tree_get_data_byte(nbt_node_t *node);

int nbt_tree_set_data_byte(nbt_node_t *node, char data);

double nbt_tree_get_data_double(nbt_node_t *node);

int nbt_tree_set_data_double(nbt_node_t *node, double data);

float nbt_tree_get_data_float(nbt_node_t *node);

int nbt_tree_set_data_float(nbt_node_t *node, float data);

short nbt_tree_get_data_short(nbt_node_t *node);

int nbt_tree_set_data_short(nbt_node_t *node, short data);

long nbt_tree_get_data_long(nbt_node_t *node);

int nbt_tree_set_data_long(nbt_node_t *node, long data);

int nbt_tree_get_data_int(nbt_node_t *node);

int nbt_tree_set_data_int(nbt_node_t *node, int data);

char *nbt_tree_get_data_str(nbt_node_t *node);

int nbt_tree_set_data_str(nbt_node_t *node, char *data);

nbt_tag_type_t nbt_tree_get_list_type(nbt_node_t *node);

nbt_node_t *nbt_tree_get_first_child(nbt_node_t *node);

nbt_node_t *nbt_tree_get_last_child(nbt_node_t *node);

nbt_node_t *nbt_tree_get_parent(nbt_node_t *node);

nbt_node_t *nbt_tree_get_next_child(nbt_node_t *node);

nbt_node_t *nbt_tree_get_prev_child(nbt_node_t *node);

int nbt_tree_append_child(nbt_node_t *parent, nbt_node_t *child);

int nbt_tree_prepend_child(nbt_node_t *parent, nbt_node_t *child);

int nbt_tree_insert_after(nbt_node_t *left, nbt_node_t *right);

int nbt_tree_insert_before(nbt_node_t *right, nbt_node_t *left);

int nbt_tree_unlink(nbt_node_t *node);

int nbt_tree_replace(nbt_node_t *old, nbt_node_t *new);

size_t nbt_tree_get_len(nbt_node_t *node);

char *_serialize_string(nbt_node_t *node, size_t *len);

#ifdef __cplusplus
}
#endif

#endif
