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

#include "Error.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static int initialized = 0;
static Error_Handler handleError;

static void HandleError(const char msg[]) /*default error handler*/
{
	assert(msg != NULL);

	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}


void Error_Init(void)
{
	if (! initialized) {
		initialized = 1;
		handleError = HandleError;
	}
}


void Error_SetHandler(Error_Handler h)
{
	assert(initialized);
	assert(h != NULL);

	handleError = h;
}


void Error_Handle(const char msg[])
{
	assert(initialized);
	assert(msg != NULL);

	handleError(msg);
}
