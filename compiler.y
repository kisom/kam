%{
#include <stdio.h>
%}

%token EOL VAL WS IMM ADD SUB MUL DIV CMP JNE

%%

program:
| program EOL { }

%%
main(int argc, char *argv[])
{
	yyparse();
}

yyerror(char *s)
{
	fprintf(stderr, "ERROR: %s\n", s);
}

