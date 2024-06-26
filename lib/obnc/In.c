/*Copyright 2017-2019, 2023, 2024 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/In.h"
#include <obnc/OBNC.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))

int In__Done_ = 0;
static int inputConsumed = 0;

void In__Open_(void)
{
	In__Done_ = ! inputConsumed;
}


void In__Char_(char *ch)
{
	int d;

	d = getchar();
	*ch = (char) d;
	In__Done_ = d != EOF;
	if (In__Done_) {
		inputConsumed = 1;
	}
}


void In__Int_(OBNC_INTEGER *x)
{
	int ch, i, n;
	char buf[(CHAR_BIT * sizeof (OBNC_INTEGER) / 3) + 3];
	unsigned OBNC_INTEGER y;

	In__Done_ = 0;
	do {
		ch = getchar();
	} while (isspace(ch));
	i = 0;
	if (ch == '-') {
		buf[i] = (char) ch;
		i++;
		ch = getchar();
	}
	if (isdigit(ch)) {
		do {
			buf[i] = (char) ch;
			i++;
			ch = getchar();
		} while ((isdigit(ch) || ((ch >= 'A') && (ch <= 'F'))) && (i < LEN(buf)));
		if (i < LEN(buf)) {
			buf[i] = '\0';
			if (ch == 'H') {
				n = sscanf(buf, "%" OBNC_INT_MOD "x", &y);
				if (n == 1) {
					*x = (OBNC_INTEGER) y;
				}
			} else {
				n = sscanf(buf, "%" OBNC_INT_MOD "d", x);
				if (ch != EOF) {
					ungetc(ch, stdin);
				}
			}
			In__Done_ = n == 1;
		}
	} else 	if (ch != EOF) {
		ungetc(ch, stdin);
	}
	if (In__Done_) {
		inputConsumed = 1;
	}
}


void In__Real_(OBNC_REAL *x)
{
	int scanCount;

	scanCount = scanf("%" OBNC_REAL_MOD_R "f", x);
	In__Done_ = scanCount == 1;
	if (In__Done_) {
		inputConsumed = 1;
	}
}


void In__String_(char str[], OBNC_INTEGER strLen)
{
	int ch, i, ord;

	In__Done_ = 0;
	do {
		ch = getchar();
	} while (isspace(ch));
	if (ch == '"') {
		i = 0;
		ch = getchar();
		while ((ch != EOF) && (ch != '"') && (ch != '\n') && (i < strLen)) {
			str[i] = (char) ch;
			i++;
			ch = getchar();
		}
		if ((ch == '"') && (i < strLen)) {
			str[i] = '\0';
			In__Done_ = 1;
		}
	} else if (isdigit(ch)) {
		ord = ch - '0';
		ch = getchar();
		while ((isdigit(ch) || ((ch >= 'A') && (ch <= 'F'))) && (ord < UCHAR_MAX)) {
			ord = isdigit(ch)? ord * 16 + ch - '0': ord * 16 + 10 + ch - 'A';
			ch = getchar();
		}
		if ((ch == 'X') && (ord <= UCHAR_MAX) && (strLen >= 2)) {
			str[0] = (char) ord;
			str[1] = '\0';
			In__Done_ = 1;
		}
	}
	if (! In__Done_) {
		str[0] = '\0';
	}
	inputConsumed = 1;
}


void In__Name_(char name[], OBNC_INTEGER nameLen)
{
	int n, ch, i;

	In__Done_ = 0;
	n = 0;
	do {
		ch = getchar();
		n++;
	} while (isspace(ch));
	if (ch != EOF) {
		i = 0;
		while ((i < nameLen) && (isgraph(ch) || ((unsigned char) ch >= 128))) {
			name[i] = (char) ch;
			i++;
			ch = getchar();
			n++;
		}
		if (i < nameLen) {
			name[i] = '\0';
			In__Done_ = ! isgraph(ch);
		} else {
			name[0] = '\0';
		}
	}
	if (ch == EOF) {
		n--;
	}
	if (n > 0) {
		inputConsumed = 1;
	}
}


void In__Line_(char line[], OBNC_INTEGER lineLen)
{
	int i, ch;

	i = 0;
	ch = getchar();
	while ((ch != EOF) && (ch != '\n')) {
		if (i < lineLen) {
			line[i] = (char) ch;
		}
		i++;
		ch = getchar();
	}
	if ((i > 0) || (ch == '\n')) {
		inputConsumed = 1;
	}
	if (i < lineLen) {
		line[i] = '\0';
		In__Done_ = 1;
	} else {
		line[lineLen - 1] = '\0';
		In__Done_ = 0;
	}
}


void In__Init(void)
{
}
