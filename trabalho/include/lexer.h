/**@<lexer.h>::**/

/**
	Grupo 06 
	integrantes: F�bio Vin�cius Luciano da Silva, Jo�o Victor Fleming, Alexandre dos Anjos Souza
**/

#pragma once
#include <stdio.h>
#define MAXIDSIZE 64
extern int lookahead;
extern char lexeme[];
int gettoken(FILE *source);
