/**@<symtab.c>::**/

/**
	Grupo 06 
	integrantes: F�bio Vin�cius Luciano da Silva, Jo�o Victor Fleming, Alexandre dos Anjos Souza
**/

/*** Aqui será implementado o que se encontra como extern no ./include/symtab.h
 *   Vão para o referido código cabeçalho ***/
 #include <string.h>
 #include <symtab.h>
 int symtab_next_entry; /** primeira posição na tabela é 0 */
 char symtab[MAXIDSIZE+1][SYMTAB_MAX_ENTRIES];

 int symtab_lookup(const char *name) {
   int i;
   for (i = 0; i < symtab_next_entry; i++) {
     if (strcmp(symtab[i],name) == 0) return i;
   }
   return -1;
 } 
 int symtab_append(const char *name) {
   int i;
   strcpy(symtab[symtab_next_entry], name);
   return i;
 }

