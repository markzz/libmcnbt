//
// Created by markzz on 3/29/18.
//

#ifndef LIBMCNBT_TREE_H
#define LIBMCNBT_TREE_H

#include "mcnbt.h"

int nbt_tree_set_len(nbt_node_t *node, size_t len);

nbt_node_t *_nbt_parse(void *data, size_t size, nbt_node_t *root, int *pos);

#endif
