/**@<parser.c>::**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tokens.h>
#include <lexer.h>
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
void store (const char *name, double val) {
	int address = symtab_lookup(name);
	if (address < 0) {
		address = syntab_append(name);
	}
	vmemory[address] = val;
}
double recall (const char *name) {
	int address = symtab_lookup(name);
	if (address < 0) {
		address = syntab_append(name);
	}
	return vmemory[address];
}
/** define um registrador ficticio **/
double acc;
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
			printf("\tpush acc\n");
			opluspush = 1;
		}
	}/***/

_F_head:
	/***/if (isotimes) {
		if (otimespush == 0) {
			printf("\tpush acc\n");
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
				/***/printf("\tstore acc, %s\n", varname);/***/
			} else {
				/** R-value **/
				/***/printf("\trecall %s, acc\n", varname);/***/
			}
			break;
		case DEC:
			printf("\tmov %s, acc\n", lexeme);
			match(DEC);
			break;
#ifdef _UNIX_DECIMAL_NOTATION_
		case OCTAL:
			printf(" %s ", lexeme);
			match(OCTAL);
			break;
		case HEXA:
			printf(" %s ", lexeme);
			match(HEXA);
			break;
#endif
		case FLT:
			printf(" %s ", lexeme);
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
				printf("\tmul acc, stack[sp]\n");
				break;
			case '/':
				printf("\tdiv acc, stack[sp]\n");
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
		printf("\tpop acc\n");
		otimespush = 0;
	}/***/

	/***/if(isominus){acc = -acc; isominus = 0;}/***/

	/***/if(isoplus){
		switch (isoplus) {
			case '+':
				/**/printf("\tadd acc, stack[sp]\n");/**/
				break;
			case '-':
				printf("\tsub acc, stack[sp]\n");
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
		printf("\tpop acc\n");
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
		fprintf(stderr,
				"parser: token inválido; era esperado %d mas foi encontrado %d\n",
				expected, lookahead);
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
			case EOF:
				match(EOF);
				return;
			default:
				E();
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
