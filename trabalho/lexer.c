/**@<lexer.c>::**/

/**
	Grupo 06 
	integrantes: F�bio Vin�cius Luciano da Silva, Jo�o Victor Fleming, Alexandre dos Anjos Souza
**/

#include <stdio.h>
#include <ctype.h>

#include <tokens.h>
#include <keywords.h>
#include <lexer.h>

/********
 * Neste módulo é onde implementamos os autômatos, ou as
 * expressões regulares numa forma bastante simples e
 * intuitiva, como apresentado na aula assíncrona 2 no
 * YouTube
 ********/

/********
 * Comecemos pelo analisador léxico de identificadores
 * Pascal primitivo
 ********/

/** REGEX: [A-Z][A-Z0-9]*
 * Forma mais flexível é "case sensitive":
 * [A-Za-z] == isalpha
 * [0-9] == isdigit
 * [A-Za-z0-9] == isalnum
 * [A-Za-z][A-Za-z0-9]*
**/
char lexeme[MAXIDSIZE+1];
int
isID(FILE * tape)
{
    int             i = 0, token;
    if (isalpha(lexeme[i] = getc(tape))) {
        /** já é um identificador **/
        /** agora, falta pegar o sufixo [A-Za-z0-9]*, se houver **/
	i++;
        while (isalnum(lexeme[i] = getc(tape))) i++;
        ungetc(lexeme[i], tape);
	lexeme[i] = 0;
	if ((token = iskeyword(lexeme))) return token;
        return ID;
    }
    ungetc(lexeme[i], tape);
    return 0;
}

/*****
 * inteiro decimal, sem zero prefixando, é o seguinte padrão:
 * [1-9][0-9]* | 0
 *****/
int
isDEC(FILE * tape)
{
    int i = 0;
#ifdef _UNIX_DECIMAL_NOTATION_
    if (isdigit(lexeme[i] = getc(tape))) {
        if (lexeme[i] == '0') {
            lexeme[i + 1] = 0;
            return DEC;
        }
	i++;
        while (isdigit(lexeme[i] = getc(tape))) i++;
        ungetc(lexeme[i], tape);
	lexeme[i] = 0;
        return DEC;
    }
#else
    // Esta a convenção Pascal de números inteiros
    if (isdigit(lexeme[i] = getc(tape))) {
        i++;
        while (isdigit(lexeme[i] = getc(tape))) i++;
        ungetc(lexeme[i], tape);
	lexeme[i] = 0;
        return DEC;
    }
#endif
    ungetc(lexeme[i], tape);
    return 0;
}

#ifdef _UNIX_DECIMAL_NOTATION_
/*** REGEX: OCTAL = 0[0-7]+ **/
int
isoctal(int chr)
{
    if ('0' <= chr && chr <= '7')
        return 1;
    return 0;
}

int
isOCTAL(FILE * tape)
{
    int            i = 0;
    lexeme[i] = getc(tape);
    if (lexeme[i] == '0') {
	i++;
        if (isoctal(lexeme[i] = getc(tape))) {
	    i++;
            while (isoctal(lexeme[i] = getc(tape))) i++;
            ungetc(lexeme[i], tape);
	    lexeme[i] = 0;
            return OCTAL;
        }
        ungetc(lexeme[i], tape);
        ungetc('0', tape);
        return 0;
    }
    ungetc(lexeme[i], tape);
    return 0;
}

/** REGEX: HEXA = 0[xX][0-9A-Fa-f]+ **/
int
isHEXA(FILE * tape)
{
    int             i = 0;
    lexeme[i] = getc(tape);
    if (lexeme[i] == '0') {
        int             x = getc(tape);
	i++; lexeme[i] = x;
        if (toupper(x) == 'X') {
	    i++;
            if (isxdigit(lexeme[i] = getc(tape))) {
		i++;
                while (isxdigit(lexeme[i] = getc(tape))) i++;
                ungetc(lexeme[i], tape);
		lexeme[i] = 0;
                return HEXA;
            }
            ungetc(lexeme[i], tape);
        }
        ungetc(x, tape);
        ungetc('0', tape);
        return 0;
    }
    ungetc(lexeme[i], tape);
    return 0;
}
#endif
/*******
 * A ideia é associar analisadores léxicos com autômatos finitos
 * determinísticos (AFD) e não determinísticos AFN).
 *
 * Vimos em Teoria da computação que AFNs são mais poderosos no
 * que tange a simplicidade de se transcrever expressões regulares
 * e mesmo gramáticas regulares. O maior problema do AFD é que este
 * não admite transições sem consumo de fita, o que faz com que a
 * transcrição de um autômato para o analisador léxico seja bastante
 * dificultada.
 *
 * Passemos então para os AFN:
 *
 * A única dificuldade de se transcrever um AFN para um lexer é o
 * problema das transições múltimplas. Podemos eliminar as transições 
 * múltiplas através de fatoração à esquerda das expressões regulares 
 * que originaram aqueles AFN. Por exemplo,
 *
 * a | a b = a ( epsilon | b ) 
 *
 * Deste exemplo, podemos escrever um AFN muito simples:
 *
 *
 *          a        b
 * --->(0)----->(1)----->((2))
 *               |
 *               ------->((3))
 *
 * Um problkema prático, e m uito interessante, que, em princípio, requer
 * backtracking, é o do reconhecedor de ponto flutuante.
 *
 * Um número ponto flutuante (decimal) é o seguinte:
 * (i) uma representação decimal com parte fracionária: 3.14159 é PF;
 * (ii) uma representação de mantissa: .14159 é PF;
 * (iii) uma representação inteira posfixada por ponto decimal: 3. é PF
 * (iv) qualque número decimal seguido de notação expoente base 10 (notação E)
 * é ponto flutuante: 3e-10 é PF.
 *
 * Podemos sintetizar o que foi enumerado com a seguinte EXPREG:
 * FLT = DEC\.[0-9]* EE? | \.[0-9]+ EE? | DEC EE
 * DEC = 0 | [1-9][0-9]*
 * EE = [eE] [\+\-]? [0-9]+
 *
 *               /---------------------------->((decimal))
 *               |                    |
 *         DEC   |   '.'      [0-9]*  v    EE
 * --->(0)----->(1)----->((2))----->((3))----->((4))
 *      | '.'                         ^
 *      |         [0-9]+              |
 *       -----------------------------
 *
 */
/** DEC **/

int
isEE(FILE * tape)
{
    int             EE;
    if (toupper(EE = getc(tape)) == 'E') {
        int             plusminus = getc(tape);
        if (plusminus == '+' || plusminus == '-');
        else {
            ungetc(plusminus, tape);
            plusminus = 0;
        }
        int             digit = getc(tape);
        if (isdigit(digit)) {
            while (isdigit(digit = getc(tape)));
            ungetc(digit, tape);
            return 1;
        }
        ungetc(digit, tape);
        if (plusminus) {
            ungetc(plusminus, tape);
        }
    }
    ungetc(EE, tape);
    return 0;
}

#include <string.h>

int
isNUM(FILE * tape)
{
    int             point,
                    digit;
    int             token;

#ifdef _UNIX_DECIMAL_NOTATION_
    token = isOCTAL(tape);
    if (token == OCTAL) {
        return OCTAL;
    }

    token = isHEXA(tape);

    if (token == HEXA) {
        return HEXA;
    }
#endif

    token = isDEC(tape);
    int i = strlen(lexeme);

    if (token == DEC) {
        point = getc(tape);
        if (point == '.') {
            token = FLT;
            while (isdigit(digit = getc(tape)));
            ungetc(digit, tape);
        } else {
            ungetc(point, tape);
        }
    } else {
        point = getc(tape);
        if (point == '.') {
            digit = getc(tape);
            if (isdigit(digit)) {
                token = FLT;
                while (isdigit(digit = getc(tape)));
                ungetc(digit, tape);
            } else {
                ungetc(digit, tape);
                ungetc(point, tape);
                return 0;
                /** não é um número **/
            }
        } else {
            ungetc(point, tape);
            return 0;
            /** não é um número **/
        }
    }
    /** checa a parte exponencial se houver porque isso é um número **/
    if (isEE(tape)) {
        return FLT;
    }
    return token;
}

/** ASGN = ":=" **/
int
isASGN(FILE * tape)
{
    int             head = getc(tape);
    if (head == ':') {
        head = getc(tape);
        if (head == '=') {
            return ASGN;
        }
        ungetc(head, tape);
        ungetc(':', tape);
        return 0;
    }
    ungetc(head, tape);
    return 0;
}

int
isRELOP(FILE * tape)
{
    /** RELOP = "=" | ">=" | "<=" | "<>"
                '=' | GEQ  | LEQ  | NEQ **/
    int             head = getc(tape);

    if (head == '<') {
        if ((head = getc(tape)) == '=') {
            return LEQ;
        } else if (head == '>') {
            return NEQ;
        }
        ungetc(head, tape);
        return '<';
    } else if (head == '>') {
        if ((head = getc(tape)) == '=') {
            return GEQ;
        }
        ungetc(head, tape);
        return '>';
    }
    ungetc(head, tape);
    return 0;
}

void
skipspaces(FILE * tape)
{
    int             head;
    while (isspace(head = getc(tape)))
#ifdef _COMMAND_INTERPRETER_
    {
        if (head == '\n')
            break;
    }
#else
	    ;
#endif
    ungetc(head, tape);
}

int
gettoken(FILE * source)
{
    int             token;

    skipspaces(source);

    if ((token = isID(source)))
        return token;

    if ((token = isNUM(source)))
        return token;

    if ((token = isASGN(source)))
        return token;

    token = getc(source);

    return token;
}
