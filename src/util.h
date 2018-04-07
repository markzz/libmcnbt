/*
 *  util.h
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

#ifndef LIBMCNBT_UTIL_H
#define LIBMCNBT_UTIL_H

void _mcnbt_alloc_fail(size_t size);

#define MALLOC(p, s, action) do { p = malloc(s); if (p == NULL) { _mcnbt_alloc_fail(s); action; } } while(0)
#define CALLOC(p, l, s, action) do { p = calloc(l, s); if(p == NULL) { _mcnbt_alloc_fail(l * s); action; } } while(0)

#define FREE(p) do { free(p); p = NULL; } while(0)

#define ASSERT(cond, action) do { if(!(cond)) { action; } } while(0)

void *_mcnbt_memcat(void *dest, const void *src, size_t n, int pos);


#endif //LIBMCNBT_UTIL_H
