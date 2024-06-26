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

%option always-interactive
%option noinput nounput
%x comment

%{
#include "Oberon.h"
#include "Util.h"
#include "../lib/obnc/OBNC.h"
#include "Trees.h" /*needed by YYSTYPE in y.tab.h*/
#include "y.tab.h"
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int commentNestingLevel;
static int commentStartLine;

static int KeywordToken(const char word[]);

%}

WORD [A-Za-z](_?[A-Za-z0-9])*

INTEGER [0-9]+|[0-9][0-9A-F]*H

REAL [0-9]+"."[0-9]*(E[+-]?[0-9]+)?

QUOTED-STRING \"[^"\n]*\"

ORDINAL-STRING [0-9][0-9A-F]*X

%%

[ \t\r]+

\n {
	yylineno++;
}

":=" return BECOMES;

".." return DOTDOT;

"<=" return LE;

">=" return GE;

[][*+/&~.,;|({^:)}=#<>-] return yytext[0];

{WORD} {
	int token;
	char *lexeme;

	token = KeywordToken(yytext);
	if (token < 0) {
		token = IDENT;
		NEW_ARRAY(lexeme, yyleng + 1);
		strcpy(lexeme, yytext);
		yylval.ident = lexeme;
	}
	return token;
}

{INTEGER}/".."? {
	int base;
	unsigned long int max, lexeme;

	base = (yytext[yyleng - 1] == 'H')? 16: 10;
	max = (yytext[yyleng - 1] == 'H')? OBNC_UINT_MAX: OBNC_INT_MAX;
	errno = 0;
	lexeme = strtoul(yytext, NULL, base);
	if ((errno != 0) || (lexeme > max)) {
		if (base == 10) {
			Oberon_PrintError("warning: %s: %s > %lu", strerror(ERANGE), yytext, max);
		} else {
			Oberon_PrintError("warning: %s: %s > 0%lXH", strerror(ERANGE), yytext, max);
		}
	}
	yylval.integer = (OBNC_INTEGER) lexeme;
	return INTEGER;
}

{REAL} {
	int n = sscanf(yytext, "%" OBNC_REAL_MOD_R "f", &yylval.real);
	if (n != 1) {
		Oberon_PrintError("warning: %s: %s > %" OBNC_REAL_MOD_W "G", strerror(ERANGE), yytext, OBNC_REAL_MAX);
	}
	return REAL;
}

{QUOTED-STRING} {
	int lexemeLen;
	char *lexeme;

	lexemeLen = yyleng - 1;
	NEW_ARRAY(lexeme, lexemeLen);
	memcpy(lexeme, yytext + 1, (size_t) (lexemeLen - 1));
	lexeme[lexemeLen - 1] = '\0';
	yylval.string = lexeme;
	return STRING;
}

{ORDINAL-STRING} {
	long ordinalNumber;
	char *lexeme;

	if (strcmp(yytext, "0X") == 0) {
		ordinalNumber = 0;
	} else {
		errno = 0;
		ordinalNumber = strtol(yytext, NULL, 16);
		if ((errno != 0) || (ordinalNumber > UCHAR_MAX)) {
			Oberon_PrintError("warning: %s: %s > 0%XX", strerror(ERANGE), yytext, UCHAR_MAX);
		}
	}
	NEW_ARRAY(lexeme, 2);
	lexeme[0] = (char) ordinalNumber;
	lexeme[1] = '\0';
	yylval.string = lexeme;
	return STRING;
}

"(*" {
	BEGIN(comment);
	commentNestingLevel = 1;
	commentStartLine = yylineno;
}

<comment>[^*(\n]+

<comment>\n {
	yylineno++;
}

<comment>"*"+/[^)]

<comment>"("+/[^*]

<comment>"(*" commentNestingLevel++;

<comment>"*)" {
	commentNestingLevel--;
	if (commentNestingLevel == 0) {
		BEGIN(INITIAL);
	}
}

<comment><<EOF>> {
	Oberon_PrintError("error: unterminated comment starting at line %d", commentStartLine);
	exit(EXIT_FAILURE);
}

. {
	if (isprint(yytext[0])) {
		Oberon_PrintError("error: unexpected character: %c", yytext[0]);
	} else {
		Oberon_PrintError("error: unexpected character: %02X (hex)", yytext[0]);
	}
	exit(EXIT_FAILURE);
}

%%

static int Cmp(const void *word, const void *keywordPtr)
{
	return strcmp((char *) word, * (char **) keywordPtr);
}


static int KeywordToken(const char word[])
{
	static const char *keywords[] = {"ARRAY", "BEGIN", "BY", "CASE", "CONST", "DIV", "DO", "ELSE", "ELSIF", "END", "FALSE", "FOR", "IF", "IMPORT", "IN", "IS", "MOD", "MODULE", "NIL", "OF", "OR", "POINTER", "PROCEDURE", "RECORD", "REPEAT", "RETURN", "THEN", "TO", "TRUE", "TYPE", "UNTIL", "VAR", "WHILE"};

	static const int keywordTokens[] = {ARRAY, BEGIN_, BY, CASE, CONST, DIV, DO, ELSE, ELSIF, END, FALSE, FOR, IF, IMPORT, IN, IS, MOD, MODULE, NIL, OF, OR, POINTER, PROCEDURE, RECORD, REPEAT, RETURN, THEN, TO, TRUE, TYPE, UNTIL, VAR, WHILE};

	const char **keywordPtr;
	ptrdiff_t pos;
	int token;

	keywordPtr = bsearch(word, keywords, LEN(keywords), sizeof keywords[0], Cmp);
	if (keywordPtr != NULL) {
		pos = keywordPtr - keywords;
		assert(pos >= 0);
		assert(pos < LEN(keywordTokens));
		token = keywordTokens[pos];
	} else {
		token = -1;
	}
	return token;
}


int yywrap(void)
{
	const int done = 1;

	return done;
}
