%{

enum yytokentype {
	EOL,
	VAL,
	WS,
	IMM,
	ADD,
	SUB,
	MUL,
	DIV,
	CMP,
	JNE
};
%}

%%
"IMM"	{ return IMM; }
"ADD"	{ return ADD; }
"SUB"	{ return SUB; }
"MUL"	{ return MUL; }
"DIV"	{ return DIV; }
"CMP"	{ return CMP; }
"JNE"   { return JNE; }
\n	{ return EOL; }
[ \t]   { return WS; }

%%
