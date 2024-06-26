/*Copyright 2017-2019, 2023, 2024 Karl Landstrom <karl@miasap.se>

This file is part of OBNC.

OBNC is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OBNC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OBNC.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef UTIL_H
#define UTIL_H

#include <gc/gc.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))

#define NEW_ARRAY(ptr, n) \
	{ \
		assert(Util_initialized); \
		assert((n) >= 0); \
		assert((size_t) (n) <= ((size_t) -1) / sizeof (ptr)[0]); \
		(ptr) = GC_MALLOC((size_t) (n) * sizeof (ptr)[0]); \
		if ((ptr) == NULL) { \
			fprintf(stderr, "Memory allocation using GC_MALLOC failed: %s\n", strerror(errno)); \
			exit(EXIT_FAILURE); \
		} \
	}

#define RENEW_ARRAY(ptr, n) \
	{ \
		assert(Util_initialized); \
		assert((n) >= 0); \
		assert((size_t) (n) <= ((size_t) -1) / sizeof (ptr)[0]); \
		(ptr) = GC_REALLOC((ptr), (size_t) (n) * sizeof (ptr)[0]); \
		if ((ptr) == NULL) { \
			fprintf(stderr, "Memory allocation using GC_REALLOC failed: %s\n", strerror(errno)); \
			exit(EXIT_FAILURE); \
		} \
	}

#define NEW(ptr) NEW_ARRAY((ptr), 1)

extern int Util_initialized; /*don't use*/

void Util_Init(void);

char *Util_String(const char format[], ...)
	__attribute__ ((format (printf, 1, 2)));

const char *Util_Replace(const char old[], const char new[], const char s[]);

#endif
