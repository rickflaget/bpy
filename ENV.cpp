#include "lexer.h"
#include "ENV.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;


void printEnvError(string variable)
	{
	cerr << "variable " << variable << " is undefined";
	exit(1);
	}
lexeme* cons(GTYPE TYPE, lexeme *carValue, lexeme* cdrValue)
	{
	lexeme *consed = new lexeme(TYPE);
	(*consed).carvalue = carValue;
	(*consed).cdrvalue = cdrValue;
	return consed;
	}
lexeme* car(lexeme* cell)
	{
	return (*cell).carvalue;
	}
lexeme* cdr(lexeme* cell)
	{
	return (*cell).cdrvalue;
	}
void setCar(lexeme* cell, lexeme* value)
	{
	(*cell).carvalue = value;
	}
void setCdr(lexeme *cell, lexeme *value)
	{
	(*cell).cdrvalue = value;
	}
bool sameVariable(lexeme *a, lexeme *b)
	{
	//cout << "made it to same variable" << endl;
	if ((*a).String.compare((*b).String) == 0)
		{
		return true;
		}
	return false;
	}
	  //type just use .type
lexeme* createEnv(void)
	{
	return extendEnv(NULL,NULL,NULL);
	}
lexeme* extendEnv(lexeme *env, lexeme* variables, lexeme* values)
	{
	return cons(ENV_TYPE,makeTable(variables,values),env);
	}

lexeme* makeTable(lexeme* variables, lexeme *values) 
	{
	return cons(TABLE ,variables,values);
	}

lexeme* lookupEnv(lexeme* env,lexeme *variable)
	{
	int i = 0;
	while (env != NULL)
		{
		lexeme* table = car(env);
		lexeme* vars = car(table);
		lexeme* vals = cdr(table);
		while (vars != NULL)
			{
			//cout << "car(vars): " << (*car(vars)).String;
			//cout << "---car(vals): " << (*car(vals)).intvalue << endl;
			if (sameVariable(variable, car(vars)))
				{
				return car(vals);
				}
			//cout << "made it thru" << endl;
			vars = cdr(vars);
			vals = cdr(vals);
			++i;
			}
		env = cdr(env);
		}
	printEnvError((*variable).String);
	return NULL;
	}
void updateEnv(lexeme *env, lexeme *variable, lexeme* value)
	{
	while (env != NULL)
		{
		lexeme* table = car(env);
		lexeme* vars = car(table);
		lexeme* vals = cdr(table);
		while (vars != NULL)
			{
			//cout << "car(vars): " << (*car(vars)).String;
			//cout << "---car(vals): " << (*car(vals)).intvalue << endl;
			if (sameVariable(variable,car(vars)))
				{
				setCar(vals,value);
				return;
				}
			vars = cdr(vars);
			vals = cdr(vals);
			}
		env = cdr(env);
		}
	printEnvError((*variable).String);
	}


lexeme* insertEnv(lexeme *variable,lexeme* value,lexeme* env)
	{
	lexeme* table = car(env);
	setCar(table, cons(GLUE,variable,car(table)));
	setCdr(table, cons(GLUE,value,cdr(table)));
	return table;
	}
void displayEnv(lexeme *env)
	{
	cout << "*******start of env**********" << endl;
	while (env != NULL)
		{
		cout << "------------------" << endl;
		lexeme* table = car(env);
		lexeme *vars = car(table);
		lexeme *vals = cdr(table);
		while (vars != NULL)
			{
			cout << "var:" << (*car(vars)).String;
			if ((*car(vals)).type == NUMBER)
				{
				cout << " " << (*car(vals)).intvalue << endl;
				}
			else if ((*car(vals)).type == STRING)
				{
				cout << " " << (*car(vals)).String << endl;
				}
			else //add in boolean logic
				{
				cout << " " << printG((*car(vals)).type) <<  endl;
				}
			vars = cdr(vars);
			vals = cdr(vals);
			}
		cout << "-------------------" << endl;
		env = cdr(env);
		}
	cout << "*********end of env**********"<< endl;
	}
void displayLocalEnv(lexeme *env)
	{
	cout << "__________________" << endl;
	lexeme* table = car(env);
	lexeme *vars = car(table);
	lexeme *vals = cdr(table);
	while (vars != NULL)
		{
		cout << "var:" << (*car(vars)).String;
		cout << " " << (*car(vals)).intvalue << endl;
		vars = cdr(vars);
		vals = cdr(vals);
		}
	cout << "__________________" << endl;
	}


