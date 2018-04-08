/*
 *  mcnbt.c
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

#include <stdio.h>
#include <stdlib.h>
#include <archive.h>
#include <archive_entry.h>

#include "mcnbt.h"
#include "tree.h"
#include "util.h"

nbt_node_t *nbt_initialize_from_file(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    long file_size;
    char *buf;
    nbt_node_t *ret;

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    MALLOC(buf, (file_size + 1) * sizeof(char), return NULL);
    fread(buf, 1, (size_t) file_size, fp);

    ret = nbt_initialize(buf, (size_t) file_size);
    fclose(fp);
    FREE(buf);
    return ret;
}

nbt_node_t *nbt_initialize(void *data, size_t size) {
    char buf[MAX_BUFFER];
    ssize_t s;

    struct archive_entry *ae;
    struct archive *a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_raw(a);
    int r = archive_read_open_memory(a, data, size);

    if (r != ARCHIVE_OK) {
        return NULL;
    }

    r = archive_read_next_header(a, &ae);
    if (r != ARCHIVE_OK) {
        return NULL;
    }

    s = archive_read_data(a, buf, sizeof(buf));

    if (s < 0) {
        return NULL;
    }

    if (s == 0) {
        return NULL;
    }

    archive_read_free(a);

    return _nbt_parse(buf, s, NULL, NULL);
}