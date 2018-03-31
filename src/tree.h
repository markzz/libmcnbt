/*
 *  tree.h
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

#ifndef LIBMCNBT_TREE_H
#define LIBMCNBT_TREE_H

#include "mcnbt.h"

int nbt_tree_set_len(nbt_node_t *node, size_t len);

nbt_node_t *_nbt_parse(void *data, size_t size, nbt_node_t *root, int *pos);

#endif
