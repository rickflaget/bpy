#include <map>
#include <string>
using namespace std;

typedef enum gtypes {IF, MAIN, PROGRAM, ELSE, WHILE, FOR, VAR, FUNCTION, CLASS, EQUAL, OR, AND, VARIABLE, NUMBER, STRING, COMMENT,
ENDofINPUT, OPAREN, CPAREN, COMMA, PLUS, TIMES, MINUS, DIVIDES, LESSTHAN, GREATERTHAN, ASSIGN, SEMICOLON, MOD, EXP, OBRACE,
 CBRACE, DOT, UNKNOWN, UMINUS, GLUE, BINARY,LIST,STATEMENTS,FUNCTIONCALL,UNARY, OPERATOR,LOGICALOPERATOR,STATEMENT,TABLE,ENV_TYPE
,CLOSURE,BOOLEAN,UNDECLARED,ARRAY,NEWLINE,NEWTAB,ANON,OBRACKET,CBRACKET,ANONCALL,NOT,NIL} GTYPE;
const char* printG(int x)
	{
	 const char *printGTYPES[] = {
	 "IF",
	"MAIN",
	"PROGRAM",
	"ELSE",
	"WHILE",
	"FOR",
	"VAR",
	"FUNCTION",
	"CLASS",
	"EQUAL",
	"OR",
	"AND",
	"VARIABLE",
	"NUMBER",
	"STRING",
	"COMMENT",
	"ENDofINPUT",
	"OPAREN",
	"CPAREN",
	 "COMMA",
	"PLUS",
	 "TIMES",
	"MINUS",
	 "DIVIDES",
	 "LESSTHAN",
	"GREATERTHAN",
	"ASSIGN",
	"SEMICOLON",
	"MOD",
	"EXP",
	 "OBRACE",
	"CBRACE",
	"DOT",
	 "UNKNOWN",
	 "UMINUS",
	"GLUE",
	"BINARY",
	"LIST",
	"STATEMENTS",
	"FUNCTIONCALL",
	"UNARY",
	"OPERATOR",
	"LOGICALOPERATOR",
	"STATEMENT",
	"TABLE",
	"ENV_TYPE",
	"CLOSURE",
	"BOOLEAN",
	"UNDECLARED",
	"ARRAY",
	"NEWLINE",
	"NEWTAB",
	"ANON",
	"OBRACKET",
	"CBRACKET",
	"ANONCALL",
	"NOT",
	"NIL"
	};
	return printGTYPES[x];
	}

const char* printINPUT(int x)
	{
	 const char *printGTYPES[] = {
		 "if",
		"main",
		"program",
		"else",
		"while",
		"for",
		"var",
		"function",
		"class",
		"eq",
		"or",
		"and",
		"VARIABLE",
		"NUMBER",
		"STRING",
		"~",
		"ENDofINPUT",
		"OPAREN",
		"CPAREN",
		 ",",
		" + ",
		 " * ",
		" - ",
		 " / ",
		 " < ",
		" > ",
		" = ",
		" ; ",
		" % ",
		" ^ ",
		 " OBRACE ",
		" CBRACE ",
		 " . ",
		 " UNKNOWN ",
		  " - ",
		"GLUE",
		"BINARY",
		"LIST",
		"STATEMENTS",
		"FUNCTIONCALL",
		"UNARY",
		"OPERATOR",
		"LOGICALOPERATOR",
		"STATEMENT",
		"TABLE",
		"ENV_TYPE",
		"CLOSURE",
		"BOOLEAN",
		"UNDECLARED",
		"ARRAY",
		"NEWLINE",
		"NEWTAB",
		"ANON",
		"[",
		"]",
		"ANONCALL",
		"!",
		"NIL"
		};
	return printGTYPES[x];
	}
