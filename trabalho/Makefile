#Makefile for the project mypas

# DIRECTIVES=-D_COMMAND_INTERPRETER_
DIRECTIVES=-D_UNIX_DECIMAL_NOTATION_

CFLAGS=-I./include -g $(DIRECTIVES)

mybc: lexer.o main.o parser.o symtab.o
	$(CC) -o $@  $^

clean:
	$(RM) *.o

mostlyclean: clean
	$(RM) mypas *~
	(cd include;make $@)

indent:
	indent -orig -nuts -ts 4 -nfca *.c
