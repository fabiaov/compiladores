/**@<symtab.h>::**/

/**
	Grupo 06 
	integrantes: F�bio Vin�cius Luciano da Silva, Jo�o Victor Fleming, Alexandre dos Anjos Souza
**/

/**
 * Aqui será modelada a tabela de símbolos, nome geral symtab[SYMTAB_MAX_ENTRIES],
 * e seus atributos estruturais e funcionais:
 * 1) char *symtab.name armazena o símbolo que foi cadastrado a partir de
 *    lexeme;
 * 2) int symtab.address armazena o descritor (índice) da memória da 
 *    calculadora -- esta é um array de double, com mesmo número de 
 *    entradas que na tabela symtab;
 * 3) int symtab_lookup(const char *name) é o atributo funcional de busca
 *    do símbolo que deve se encontrar na tabela -- se não for encontrado,
 *    a função deve sinalizar um erro para que o símbolo seja cadastrado]
 *    como uma nova entrada, seja para consulta, seja para armazenamento --
 *    vejam como funciona no comando interativo "bc" do Unix/Linux;
 * 4) int symtab_append(const char *name) põe no apêndice de symtab o novo
 *    símbolo name -- esta função deve ser invocada sempre que o símbolo 
 *    em lexeme ainda não existir, vide Item 3;
 * 5) int symtab_next_entry é um ponteiro (cursor) que aponta para a próxima
 *    entrada disponível em symtab (defina o tamanho máximo da tabela com
 *    #define SYMTAB_MAX_ENTRIES <tamanho definido pelo programador> -- por
 *    exemplo, #define SYMTAB_MAX_ENTRIES 1024 só vai admitir um máximo de
 *    1024 variáveis da calculadora mybc;
 * 6) double vmemory[SYMTAB_MAX_ENTRIES] é a memória virtual da calculadora
 *    e deve estar casada (mesmo descritor) com a tabela symtab[SYMTAB_MAX_ENTRIES].
 *
 *    Com base no que foi descrito acima, symtab é sempre consultada toda vez que um
 *    símbolo (name) aparece no parser (vide linhas onde isso deve ser considerado --
 *    dica: antigos store e recall). Se o símbolo existir, este é consultado pela 
 *    pseudo instrução "recall" (isso é denotado por uma função C de mesmo nome). Se
 *    o símbolo não existir, seja para consulta (recall), seja para armazenamento 
 *    (store), este símbolo deve ser automaticamente cadastrado -- qual deve ser o 
 *    valor inicial?
 *
 *    Uma vez implementado o mecanismo de uso da tabela de símbolos, estudem as 
 *    linhas do parser onde serão inseridas as ações semânticas dirigidas por 
 *    sintaxe para busca e manutenção da tabela de símbolos -- isso será essencial
 *    para o funcionamento da calculadora mybc.
 *
 *    Na aula de hoje, façam testes. Inicialmente para um interpretador simples
 *    de expressões, cujo símbolo inicial é E no diagrama sintático, ou na gramática
 *    LL(1). Em seguida, vocês farão testes com o diagrama sintático (gramática) mais 
 *    abrangente, que usa o não terminal mybc como símbolo inicial -- este denota
 *    o interpretador propriamente dito a menos de uns detalhes operacionais que ficarão
 *    como pesquisa. */


/*** Parte implementada somente no ./include/symtab.h: ***/

#include <lexer.h> /** por causa da constante MAXIDSIZE: **/
typedef struct __symtab__ {
	char name[MAXIDSIZE+1];
	int address;
} SYMTAB;
#define SYMTAB_MAX_ENTRIES 0x100000 /** 1 mega posições na tabela */ 
extern char symtab[MAXIDSIZE+1][SYMTAB_MAX_ENTRIES];

/*** Parte a ser implementada no symtab.c: ***/

// extern SYMTAB symtab[SYMTAB_MAX_ENTRIES]; /** as SYMTAB_MAX_ENTRIES instâncias da tabela de símbolos */
extern int symtab_lookup(const char *name); /** busca o nome e retorna a posição encontrada; se não encontrar, retorna -1 */
extern int symtab_append(const char *name); /** concatena (põe no apêndice) a nova entrada (cuidado com duplicatas) */
extern int symtab_next_entry; /** primeira posição na tabela é 0 */
extern double vmemory[SYMTAB_MAX_ENTRIES]; /** esta é a memória virtual da calculadora */
/** Esses extern são porque não se implementa código em arquivos de cabeçalho, *.h -- copie para o symtab.c e retirem o extern -- 
 *  lá vocês implementam as funções e depois transformem suas chamadas e acesso aos seus atributos na forma de ações semânticas
 *  no parser.c (estudem criteriosamente as linhas onde essas ações aparecerão). As declarações de tipo e definição de constante
 *  permanecem neste código e serão utilizadas pelo symtab.c através de #include <symtab.h>
 *
 *
 ***  ***/

