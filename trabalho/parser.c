/**@<parser.c>::**/

/**
	Grupo 06 
	integrantes: F�bio Vin�cius Luciano da Silva, Jo�o Victor Fleming, Alexandre dos Anjos Souza
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tokens.h>
#include <keywords.h>
#include <lexer.h>
#include <symtab.h>
#include <parser.h>
/*
 Nesta aula, vamos aproveitar a versão da aula passada para implementarmos
 uma forma simplificada de uma tabela de símbolos. Para tanto, criem um código
 symtab.c e seu respectivo cabeçalho ./include/symtab.h
 Não esqueça de incluir, entre as linhas 5 e 6 a diretiva de inclusão
 #include <symtab.h>
 Abram o código (ainda não implementado) symtab.c e passemos para lá.
 
 Depois de implementarem a parte da tabela de símbolos e suas funcionalidades
 modifiquem este código para que as ações semânticas não sejam mais de escrita
 no padrão de saída mas de invocação de funções C de execução de operações 
 aritméticas e manutenção da pilha, bem como as pseudo instruções de busca (recall)
 e armazenamento (store) na memória virtual da calculadora -- após essa tarefa, vocês
 terão o essencial do interpretador de comandos para calculadora mybc, de modo semelhante
 ao interpretador de comandos bc do Unix.
 
 Tempo previsto para a atividade completa: 2 (duas) horas
 */

int lookahead;

#define oplus lookahead == '+' || lookahead == '-'
#define otimes lookahead == '*' || lookahead == '/'
#define ominus lookahead == '-'
double vmemory[SYMTAB_MAX_ENTRIES]; /** esta é a memória virtual da calculadora */


void store (const char *name, double val) {
	int address = symtab_lookup(name);
	if (address < 0) {
		address = symtab_append(name);
	}
	vmemory[address] = val;
}
double recall (const char *name) {
	int address = symtab_lookup(name);
	if (address < 0) {
		address = symtab_append(name);
	}
	return vmemory[address];
}
/** define um registrador ficticio **/
double acc;
/*** define uma pilha ficticia ***/
#define MAXSTACKSIZE 1024
double stack[MAXSTACKSIZE];
int sp = -1;

void push (double val) {
	sp++;
	stack[sp] = val;

}
double pop (void) {
	double aux = stack[sp];
	sp--;
	return aux;
}

int printresult = 1;

void
E(void)
{
	/***/int isominus = 0, isotimes = 0, isoplus = 0, otimespush = 0, opluspush = 0;/***/
	/***/char varname[MAXIDSIZE+1];/***/

	if (ominus) {
		/***/isominus = lookahead;/***/

		match('-');
	}

_T_head:
	/***/if (isoplus) {
		if (opluspush == 0) {
			/**/ push(acc); /**/
			opluspush = 1;
		}
	}/***/

_F_head:
	/***/if (isotimes) {
		if (otimespush == 0) {
			/**/ push(acc); /**/
			otimespush = 1;
		}
	}/***/

	switch (lookahead) {
		case ID:
			/***/strcpy(varname, lexeme);/***/
			match(ID);
			if (lookahead == ASGN) {
				/** L-value **/
				match(ASGN);
				E();
				/***/
				printresult = 0;
				store(varname, acc);
				//printf("\tstore acc, %s\n", varname);
				/***/
			} else {
				/** R-value **/
				acc = recall(varname);
				// /***/printf("\trecall %s, acc\n", varname);/***/
			}
			break;
		case DEC:
			/***/ acc = atoi(lexeme);/***/
			match(DEC);
			break;
#ifdef _UNIX_DECIMAL_NOTATION_
		case OCTAL:
			/***/ acc = atoi(lexeme);/***/
			match(OCTAL);
			break;
		case HEXA:
			/***/ acc = atoi(lexeme);/***/
			match(HEXA);
			break;
#endif
		case FLT:
			/***/ acc = atof(lexeme);/***/
			match(FLT);
			break;
		default:
			match('(');
			E();
			match(')');
	}
	// end of factor

	/*** Vezes, "*", é mul e dividido, "/", é div ***/
	/***/if(isotimes){
		switch (isotimes) {
			case '*':
				/***/ stack[sp] *= acc; /***/
				break;
			case '/':
				/***/ stack[sp] /= acc; /***/
				break;
		}
		isotimes = 0;
	}/***/

	if (otimes) {

		/***/isotimes = lookahead;/***/

		match(lookahead); goto _F_head;
	}

	// end of term

	/***/if (otimespush) {
		/**/ acc = pop(); /**/
		otimespush = 0;
	}/***/

	/***/if(isominus){acc = -acc; isominus = 0;}/***/

	/***/if(isoplus){
		switch (isoplus) {
			case '+':
				/***/ stack[sp] += acc;/***/
				break; 
			case '-':
				/***/ stack[sp] -= acc;/***/
				break;
		}
		isoplus = 0;
	}/***/

	if (oplus) {

		/***/isoplus = lookahead;/***/

		match(lookahead); goto _T_head;
	}

	// end of E

	/***/if (opluspush) {
		/**/ acc = pop(); /**/
		opluspush = 0;
	}/***/

}

FILE           *source;

	void
match(int expected)
{
	if (expected == lookahead) {
		lookahead = gettoken(source);
	} else {
		if ((expected >= ID && lookahead <= NEQ) && (lookahead >= ID && lookahead <= NEQ )){
		
		fprintf(stderr,
				"parser: token inválido; era esperado %s mas foi encontrado %s\n",
				token[expected - ID], token[lookahead - ID]);
		}
		exit(-2);
	}
}

#ifdef _COMMAND_INTERPRETER_

/*** Definição sintática de comando ***/
/*
 * O conceito de comandos para a calculadora é o seguinte:
 *
 * cmd -> E [;\n] cmd | epsilon [;\n] cmd | E EOF | EOF
 *
 */

	void
cmd(void)
{
	while (1) {
		switch (lookahead) {
			case ';':
				match(';');
				break;
			case '\n':
				match('\n');
				break;
			case QUIT:
			case EXIT:
			case BYE:
				match(QUIT);
				return;
			case EOF:
				match(EOF);
				return;
			default:
				/***/
				printresult = 1;
				/***/
				
				E();
				
				/***/
				if (printresult) printf("%lg\n", acc);
				/***/
				
				switch (lookahead) {
					case ';':
						match(';');
						break;
					case '\n':
						match('\n');
						break;
					default:
						match(EOF);
						return;
				}
		}
	}
}
#endif
