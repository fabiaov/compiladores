# Grupo 06 
# integrantes: Fábio Vinícius Luciano da Silva, João Victor Fleming, Alexandre dos Anjos Souza
#Makefile for the project myBc

# DIRECTIVES=-D_COMMAND_INTERPRETER_
DIRECTIVES=-D_COMMAND_INTERPRETER_ -D_UNIX_DECIMAL_NOTATION_
#DIRECTIVES=-D_UNIX_DECIMAL_NOTATION_

CFLAGS=-I./include -g $(DIRECTIVES)

mybc: lexer.o main.o parser.o symtab.o keywords.o tokens.o
	$(CC) -o $@  $^

clean:
	$(RM) *.o

mostlyclean: clean
	$(RM) mypas *~
	(cd include;make $@)

indent:
	indent -orig -nuts -ts 4 -nfca *.c
