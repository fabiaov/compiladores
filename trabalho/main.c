/**@<main.c>::**/

/**
	Grupo 06 
	integrantes: F�bio Vin�cius Luciano da Silva, Jo�o Victor Fleming, Alexandre dos Anjos Souza
**/

#include <stdio.h>
#include <tokens.h>
#include <main.h>
extern double acc;
int
main(void)
{
    /** isso aqui é o boot pro parser **/
    lookahead = gettoken(source = stdin);

    /** invoca o símbolo inicial da gramática **/
#ifdef _COMMAND_INTERPRETER_
    cmd();
#else
    E();
    printf("%lg\n", acc);
#endif

    printf("\n");

    return 0;
}
