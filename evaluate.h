#ifndef EVALUATE_H
#define EVALUATE_H
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

lexeme* eval(lexeme *tree, lexeme* env);
lexeme* getFuncName(lexeme *tree);
lexeme* evalGlue(lexeme *tree,lexeme *env);
	
lexeme* evalProgram(lexeme *tree, lexeme *env);
void evalFuncDef(lexeme *tree, lexeme *env); //should this be void??
lexeme* getFuncCallArgs(lexeme *tree);
lexeme* getClosureParams(lexeme *closure);
lexeme *getClosureBody(lexeme *closure);
lexeme *getClosureEnvironment(lexeme *closure);
lexeme *newStringLexeme(string s);
lexeme *newLexeme(lexeme *tree);
lexeme *evalList(lexeme *tree, lexeme *env);
lexeme *evalArgs(lexeme *args,lexeme *env);
lexeme* evalFuncCall(lexeme *tree, lexeme *env);
lexeme* evalBlock(lexeme *tree, lexeme *env);
lexeme* evalSimpleOp(lexeme *tree, lexeme *env);
lexeme* evalAnd(lexeme*, lexeme*);
lexeme* evalOr(lexeme*, lexeme*);
lexeme* evalGreaterThan(lexeme*, lexeme*);
lexeme* evalLessThan(lexeme*, lexeme*);
lexeme* newNumberLexeme(int value);
lexeme* evalPlus(lexeme *tree,lexeme *env);
lexeme* evalTimes(lexeme *tree,lexeme *env);
lexeme* evalDivides(lexeme *tree,lexeme *env);
lexeme* evalMinus(lexeme *tree,lexeme *env);
lexeme* evalMod(lexeme *tree,lexeme *env);
lexeme* evalExp(lexeme *tree,lexeme *env);
lexeme* newBooleanLexeme(bool value);
lexeme* evalEqual(lexeme *tree,lexeme *env);
lexeme* evalIf(lexeme*,lexeme*);
lexeme* evalWhile(lexeme*,lexeme*);
void evalPrintHelper(int);
void evalPrintHelper(string);
void evalPrintHelper(bool);
void evalPrint(lexeme *eargs);
void evalAssign(lexeme *tree, lexeme *env);
void evalVarDef(lexeme *tree, lexeme*env);
lexeme *evalCreateArray(lexeme *);
lexeme *evalGetArray(lexeme *);
void evalSetArray(lexeme *);
void viewArray(lexeme *);
void evalShowArray(lexeme*);
lexeme* evalAnonDef(lexeme*,lexeme*);
lexeme* evalAnonCall(lexeme*,lexeme*);
lexeme *evalGetInput(lexeme*);
lexeme *evalUMinus(lexeme *, lexeme*);
lexeme *evalIsInt(lexeme *);
lexeme *evalNot(lexeme *,lexeme *);
lexeme *evalStrCompare(lexeme *);
#endif	



		
