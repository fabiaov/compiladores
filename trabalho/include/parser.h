/** <parser.h>:: **/
extern char lexeme[];
extern int lookahead;

int gettoken(FILE *);
void match(int);

void store(const char *name, double val);
double recall(const char *name);
void push (double val);
double pop (void);