/** <parser.h>:: **/

/**
	Grupo 06 
	integrantes: F�bio Vin�cius Luciano da Silva, Jo�o Victor Fleming, Alexandre dos Anjos Souza
**/

extern char lexeme[];
extern int lookahead;

int gettoken(FILE *);
void match(int);

void store(const char *name, double val);
double recall(const char *name);
void push (double val);
double pop (void);
