#include <vector>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <ctype.h>
#include "grammartypes.h"
#include <stdlib.h>
using namespace std;

class lexeme 
	{
	public:
	GTYPE type;
	string String;
	int intvalue;
	double real;
	bool boolean;
	vector<lexeme*> array;
	lexeme *right;
	lexeme *left;
	lexeme *cdrvalue;
	lexeme *carvalue;
	lexeme(GTYPE t)
		{
		type = t;
		String = "";
		intvalue = -9999;
		real = -9999.00;
		boolean = true; //for evaluated expressions in while functions
		//array = NULL;
		right = NULL;
		left = NULL;
		cdrvalue = NULL;
		carvalue = NULL;
		};
	lexeme(GTYPE t, string s)
		{
		type = t;
		String = s;
		intvalue = -9999;
		real = -9999.00;
		boolean = false;
		//array = NULL;
		right = NULL;
		left = NULL;
		cdrvalue = NULL;
		carvalue = NULL;
		};
	lexeme(GTYPE t, string s, int k)
		{
		type = t;
		intvalue = k;
		String = s;
		real = -9999.00;
		boolean = false;
		right = NULL;
		left = NULL;
		//array = NULL;
		cdrvalue = NULL;
		carvalue = NULL;
		};
	lexeme(GTYPE t,string s, bool b)
		{
		type = t;
		String = s;
		boolean = b;
		intvalue = -9999;
		real = -9999.00;
		right = NULL;
		left = NULL;
		cdrvalue = NULL;
		carvalue = NULL;
		//array = NULL; 
		};
	void display(void)
		{
		cout << printG(type);
		if (!(String.empty()))
			{
			cout << ": " << String;
			}
		if (type == NUMBER)
			{
			cout << ": " << intvalue;
			}
		cout << endl;
		}
	void displayArray(void) //used for testing
		{
		cout << "slot0:" << array[0] << endl;
		cout << "slot1:" << array[1] << endl;
		}
	private:

	};

class lexer
	{
	public:
	ifstream Input;
	lexer();
	lexer(char* IN)
		{
	        Input.open(IN);
		};
	void skipWhiteSpace(char ch)
		{
		if (isspace(ch)) {Input >> ch;}
		Input.putback(ch);
		}

	lexeme* lexVariableOrKeyword(char ch)
		{
		string token = "";
		while (isalpha(ch) || isdigit(ch))
			{
			token = token + ch; //grow the token string
			Input.get(ch);
			}
		//push back the character that got us out of the loop
		//it may be some kind of punctuation
		
		Input.putback(ch);
		
		//token holds either a variable or a keyword, so figure it out

		if (token.compare("if") == 0) {return new lexeme(IF);}
		else if (token.compare("program") == 0) return new lexeme(PROGRAM);
		else if (token.compare("else") == 0) return new   lexeme(ELSE);
		else if (token.compare("while") == 0) return new  lexeme(WHILE);
		else if (token.compare("for") == 0) return  new lexeme(FOR);
		else if (token.compare("var") == 0) return  new lexeme(VAR);
		else if (token.compare("function") == 0) return new  lexeme(FUNCTION);
		else if (token.compare("anon") == 0) return new  lexeme(ANON);
		else if (token.compare("class") == 0) return  new lexeme(CLASS);
		else if (token.compare("eq") == 0) return new lexeme(EQUAL);
		else if (token.compare("or") == 0) return new lexeme(OR);
		else if (token.compare("and") == 0) return new lexeme(AND);
		else if (token.compare("true")==0) return new lexeme(BOOLEAN, "true", true);
		else if (token.compare("false")==0) return new lexeme(BOOLEAN, "false", false);
		else if (token.compare(">line")==0) return new lexeme(NEWLINE);
		else if (token.compare(">line")==0) return new lexeme(NEWTAB);
		else if (token.compare("nil")==0) return new lexeme(NIL);
		//more keyword testing here
		else return new  lexeme(VARIABLE, token);
		}
	lexeme* lexNumber(char ch)
		{
		string number;
		while (isdigit(ch))
			{
			number = number + ch;
			Input.get(ch);
			}
		
		Input.putback(ch);
		//cout << number << endl;
		int j = atoi(number.c_str());
		//int k = j + 1;
		//cout << k << endl;
		lexeme *toReturn = new lexeme(NUMBER, number);
		(*toReturn).intvalue = j;
		return toReturn; 
		}
	lexeme* lexString(char ch)
		{
		string String;// = "\"";
		Input.get(ch);
		while (ch != '\"')
			{
			String = String + ch;
			Input.get(ch);	
			}
		//String = String + ch;
		return new lexeme(STRING, String);
		}
	lexeme* lexComment(char ch)
		{
		string comment = "";
		Input.get(ch);
		while (ch != '~')
			{
			comment = comment + ch;
			Input.get(ch);
			}
		return new lexeme(COMMENT, comment);
		} 
		 	
	lexeme* lex()
		{
		char ch;
		Input.get(ch);
		skipWhiteSpace(ch);
		if (!(Input >> ch)) return new lexeme(ENDofINPUT);
		switch(ch)
			{
			//single character tokens
			case '(': return new lexeme(OPAREN);
			case ')': return new lexeme(CPAREN);
			case ',': return new lexeme(COMMA);
			case '+': return new lexeme(PLUS); 
			case '*': return new lexeme(TIMES);
			case '-': return new lexeme(MINUS);
			case '/': return new lexeme(DIVIDES);
			case '<': return new lexeme(LESSTHAN);
			case '>': return new lexeme(GREATERTHAN);
			case '=': return new lexeme(ASSIGN);
			case ';': return new lexeme(SEMICOLON);
			case '%': return new lexeme(MOD);
			case '^': return new lexeme(EXP);
			case '{': return new lexeme(OBRACE);
			case '}': return new lexeme(CBRACE);
			case '.': return new lexeme(DOT);
			case '[': return new lexeme(OBRACKET);
			case ']': return new lexeme(CBRACKET);
			case '!': return new lexeme(NOT);
			//add more cases
			default:
				//multi-character tokens (only numbers,
				// variables/keyworkds, and strings)
				if (isdigit(ch))
					{
					return lexNumber(ch);
					}
				else if (isalpha(ch))
					{
					return lexVariableOrKeyword(ch);
					}
				else if (ch == '\"')
					{
					return lexString(ch);
					}
				else if (ch == '~')
					{
					return lexComment(ch);
					}
			else {return new lexeme(UNKNOWN, "UNKNOWN");}
			}
		}
	};
void scanner(char* filename)
	{
	lexeme * token = new lexeme(UNKNOWN);
	lexer * i = new lexer(filename) ;
	token = (*i).lex();
	while ((*token).type != ENDofINPUT)
		{
		(*token).display();
		token =(*i).lex();
		}
	 
	}
/*
int main (int argc, char* argv[])
	{
	scanner(argv[1]);	
	return 0;
	} */ 
