#ifndef ENV_H
#define ENV_H
#include "lexer.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;
void printEnvError(string variable);
lexeme* cons(GTYPE TYPE, lexeme *carValue, lexeme* cdrValue);
lexeme* car(lexeme* cell);
lexeme* cdr(lexeme* cell);
void setCar(lexeme* cell, lexeme* value);
void setCdr(lexeme *cell, lexeme *value);
bool sameVariable(lexeme *a, lexeme *b);
lexeme* createEnv(void);
lexeme* extendEnv(lexeme *env, lexeme* variables, lexeme* values);
lexeme* makeTable(lexeme* variables, lexeme *values); 
lexeme* lookupEnv(lexeme* env,lexeme *variable);
void updateEnv(lexeme *env, lexeme *variable, lexeme* value);
lexeme* insertEnv(lexeme *variable,lexeme* value,lexeme* env);
void displayEnv(lexeme *);
void displayLocalEnv(lexeme*);
#endif

