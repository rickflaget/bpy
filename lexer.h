#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "grammartypes.h"
using namespace std;
class lexeme
	{
	public:
	GTYPE type;
	string String;
	int intvalue;
	double real;
	bool boolean;
	vector <lexeme*> array;
	lexeme *right;
	lexeme *left;
	lexeme *cdrvalue;
	lexeme *carvalue;
	lexeme(GTYPE);
	lexeme(GTYPE,string);
	lexeme(GTYPE,string,int);
	lexeme(GTYPE,string,bool);
	void display(void);
	void displayArray(void);
	private:
	};
class lexer
	{
	public:
	ifstream Input;
	lexer();
	lexer(char* );
	void skipWhiteSpace(char );
	lexeme* lexVariableOrKeyword(char );
	lexeme* lexNumber(char );
	lexeme* lexString(char );
	lexeme* lexComment(char );
	lexeme* lex();
	};
void scanner(char* );
#endif
