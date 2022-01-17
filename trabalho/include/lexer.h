/**@<lexer.h>::**/
#pragma once
#include <stdio.h>
#define MAXIDSIZE 64
extern int lookahead;
extern char lexeme[];
int gettoken(FILE *source);
