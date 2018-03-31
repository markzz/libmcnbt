/*
 *  walker.h
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

#include "mcnbt.h"

nbt_node_t *nbt_tree_get_next(nbt_node_t *node) {
    nbt_node_t *ret = NULL;
    nbt_node_t *tmp = NULL;

    if ((ret = nbt_tree_get_first_child(node)) != NULL) {
        return ret;
    } else if ((ret = nbt_tree_get_next_child(node)) != NULL) {
        return ret;
    } else {
        tmp = nbt_tree_get_parent(node);
        while ((ret = nbt_tree_get_next_child(tmp)) == NULL) {
            tmp = nbt_tree_get_parent(tmp);
            if (tmp == NULL) {
                return ret;
            }
        }

        return ret;
    }
}

nbt_node_t *nbt_tree_get_prev(nbt_node_t *node) {
    nbt_node_t *ret = NULL;
    nbt_node_t *tmp = NULL;

    if ((tmp = nbt_tree_get_prev_child(node)) != NULL) {
        ret = nbt_tree_get_last_child(tmp);
        if (ret == NULL) {
            return tmp;
        }

        while ((tmp = nbt_tree_get_last_child(tmp)) != NULL) {
            ret = tmp;
        }

        return ret;
    } else {
        ret = nbt_tree_get_parent(node);
        return ret;
    }
}

nbt_node_t *nbt_tree_get_root(nbt_node_t *node) {
    nbt_node_t *ret = node;
    nbt_node_t *tmp = node;

    while ((tmp = nbt_tree_get_parent(tmp)) != NULL) {
        ret = tmp;
    }

    return ret;
}