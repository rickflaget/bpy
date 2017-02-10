#include <vector>
#include "lexer.h"
#include "ENV.h"
#include "evaluate.h"
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
lexeme* eval(lexeme *tree, lexeme* env)
	{
	if (tree == NULL)
		{
		return NULL;
		}
	switch ((*tree).type)
		{
		case NUMBER: return tree;
		case STRING: return tree;
		case BOOLEAN: return tree;
		case NIL: return tree;
		//find value of variables in the environment
		case VARIABLE: {
	//			displayEnv(env);
	//			cout << "Going to look up variable: " << (*tree).String << endl;
				return lookupEnv(env,tree);
				}
		//parenthesized expression
		case OPAREN: return eval((*tree).right,env);
		case BINARY: return evalSimpleOp(tree,env);
		//dot operator evals lhs, right hand side a variable
		//case DOT: return evalDot(tree,env);
		//assign operator evals rhs for sure
		//	lhs is a variable or a dot operation
		//variable and function definitions
		case VAR: {
			 evalVarDef(tree,env);
			 return NULL;
			}
		case FUNCTION: {
			evalFuncDef(tree,env);
			return NULL;
			}
		case ANON: 
			{
			return evalAnonDef(tree,env);
			}
		case ANONCALL:
			{
			return evalAnonCall(tree,env);
			}	
		//imperative constructs
		case IF: return evalIf(tree,env);
		case ELSE: return eval((*tree).right,env); //evaluate block or if, left is null
		case WHILE: return evalWhile(tree,env);
		//case FOR: return evalFor(tree,env);
		//function calls
		case FUNCTIONCALL:
				{
				lexeme *toReturn = evalFuncCall(tree,env);
				return eval(toReturn,env);
				}
		case OBRACE: return evalBlock(tree,env);
		case PROGRAM: 
				{
				lexeme *toReturn = evalProgram(tree,env);
				return toReturn;
				}
		case GLUE: 
				{
				return evalGlue(tree,env);
				}
		case STATEMENTS:
				{
				return evalGlue(tree,env); //equivilent to glue
				}
		case ARRAY:
				{
				return tree;
				}
		case UMINUS:
				{
				return evalUMinus(tree,env);
				}
		case NOT:
				{
				return evalNot(tree,env);
				}
		
		default: 
			{
			cerr << "ERROR BAD EXPRESSION: " << printG((*tree).type) <<  endl;
			exit(1);
			}
		}
	return NULL;
	}
lexeme *evalUMinus(lexeme *tree, lexeme *env)
	{
	return newNumberLexeme(0 - (*(*tree).right).intvalue);	
	}
lexeme *evalNot(lexeme *tree,lexeme *env)
	{
	lexeme *b = eval((*tree).right,env);	
	if ((*b).boolean == true)
		{
		return newBooleanLexeme(false);
		}
	else
		{
		return newBooleanLexeme(true);
		}
	}

lexeme* getFuncName(lexeme *tree)
	{
	if ((*(*tree).left).type == VARIABLE)
		{
		return (*tree).left;
		}
	else
		{
		cerr << "ERROR BAD FUNCTION NAME" << endl;
		exit(1);
		}
	}
void toReturnFunction(lexeme *toReturn)
	{
	if (toReturn == NULL)
		{
		return;
		}
	if ((*toReturn).type == NUMBER)
		{
		cout << (*toReturn).intvalue << endl;
		}
	else if ((*toReturn).type == STRING)
		{
		cout << (*toReturn).String << endl;
		}
	else if ((*toReturn).type == BOOLEAN)
		{
		if ((*toReturn).boolean == true)
			{
			cout << "true" << endl;
			}
		else
			{
			cout << "false" << endl;
			}
		}
	}
	
lexeme* evalGlue(lexeme *tree,lexeme *env)
	{
	eval((*tree).left,env);
	return eval((*tree).right,env);
	}
	
lexeme* evalProgram(lexeme *tree, lexeme *env)
	{
	eval((*tree).left,env);
	//toReturnFunction(toReturnLeft); //return values from functionCalls in top level	
	lexeme *toReturnRight = eval((*tree).right,env);
	return toReturnRight;
	}			
void evalFuncDef(lexeme *tree, lexeme *env) //should this be void??
	{
	lexeme *closure = cons(CLOSURE,env,tree);
	insertEnv(getFuncName(tree),closure,env); //was (env,getFuncDefName(t),closure)
	}

lexeme* evalAnonDef(lexeme *tree,lexeme *env)
	{
	return cons(CLOSURE,env,tree);
	}
lexeme* makeList(lexeme *tree, bool first)
	{
	if (tree == NULL)
		{
		return NULL;
		}
	if ((*tree).type == OPAREN || (*tree).type == OBRACE)
		{
		return makeList((*tree).right, true);
		}
	else if ((*tree).type == GLUE || (*tree).type == LIST)
		{
		return cons(GLUE,(*tree).left,makeList((*tree).right, false));
		}
	else if ((*tree).type == ANON)
		{
		return cons(GLUE,tree,NULL);
		}
	else 
		{
		if (first == true)
			{
			return cons(GLUE,tree,makeList((*tree).right,false));
			}
		else
			{
			return cons(GLUE,tree,NULL);
			}
		}
	return NULL;
	}
lexeme* getFuncCallArgs(lexeme *tree)
	{
	return (*tree).right;
	}
lexeme* getClosureParams(lexeme *closure)
	{
	lexeme *paramTree = (*cdr(closure)).right;
	return (*paramTree).left; //optParamList
	}
lexeme *getClosureBody(lexeme *closure)
	{
	lexeme *bodyTree = (*cdr(closure)).right;
	return (*bodyTree).right; //block
	} 
lexeme *getClosureEnvironment(lexeme *closure)
	{
	return car(closure);
	}
lexeme *newStringLexeme(string s)
	{
	lexeme *str = new lexeme(STRING);
	(*str).String = s;
	return str;
	}
lexeme *newLexeme(lexeme *tree)
	{
	if (tree == NULL)
		{
		return NULL;
		}
	if ((*tree).type == NUMBER)
		{
		newNumberLexeme((*tree).intvalue);
		}
	else if ((*tree).type == STRING)
		{
		newStringLexeme((*tree).String);
		}
	else if ((*tree).type == BOOLEAN)
		{
		newBooleanLexeme((*tree).boolean);
		}
	else
		{
		cerr <<"not bool,int,or string" << endl;
		exit(-1);
		}
	return NULL;
	}

lexeme *evalList(lexeme *tree, lexeme *env)
	{		//DELETE WHEN HAVE FREE TIME
	cout << "THIS SHOULD NOT BE USED" << endl;
	lexeme *lvalue = newLexeme((eval((*tree).left,env)));
	return lvalue;
	}
	
lexeme *evalArgs(lexeme *args,lexeme *env)
	{
	lexeme *toReturn = args;
	while (args != NULL)
		{
		setCar(args,eval(car(args),env));
		args = cdr(args);
		}
	return toReturn;
	}
lexeme* evalAnonCall(lexeme *tree,lexeme *env)
	{
	lexeme *args = (*tree).left;
	args = makeList(args,true);
	lexeme *eargs = evalArgs(args,env);
	lexeme *params = (*(*tree).right).left;
	params = makeList(params,true);	
	lexeme *body = (*(*tree).right).right;
	lexeme *xenv = extendEnv(env,params,eargs);
	return eval(body,xenv);
	}

lexeme* evalFuncCall(lexeme *tree, lexeme *env)
	{
	lexeme *funcName = getFuncName(tree);
	//(*funcName).display();	
	lexeme *args = getFuncCallArgs(tree);
	args = makeList(args,true);
	lexeme *eargs = evalArgs(args,env);
	//check for built-ins here
	if ((*funcName).String.compare("showEnv") == 0) //used for testing language
		{
		displayEnv(env);
		return NULL;
		}	
	if ((*funcName).String.compare("isInt") ==0)//returns true if integer
		{
		return evalIsInt(eargs);
		}
	if ((*funcName).String.compare("getIn") ==0) //gets input from stdin with "std"
		{
		return evalGetInput(eargs);
		}
	if ((*funcName).String.compare("print")==0)
		{
		evalPrint(eargs);
		return NULL;
		}
	if ((*funcName).String.compare("strCompare")==0) //functions same as C+= .Compare()
		{
		return evalStrCompare(eargs);
		}
	if ((*funcName).String.compare("createArray")==0)
		{
		return evalCreateArray(eargs);
		} 
	if ((*funcName).String.compare("getArray")==0)
		{
		return evalGetArray(eargs);
		}
	if ((*funcName).String.compare("setArray")==0)
		{
		evalSetArray(eargs);
		return NULL;
		}
	lexeme *closure = eval(funcName,env);
	lexeme *params = getClosureParams(closure);
	params = makeList(params, true);
	lexeme *body = getClosureBody(closure);
	//(*body).display();
	lexeme *senv = getClosureEnvironment(closure);
	lexeme *xenv = extendEnv(senv,params,eargs);
	return eval(body,xenv);
	}	
	
lexeme* evalBlock(lexeme *tree, lexeme *env)
	{//block closure???
	lexeme *result;
	result = eval((*tree).right, env);
	//while (tree != NULL)
	//	{
	//	result = eval((*tree).left, env);
	//	tree = (*tree).right;
	//	}
	return result;
	}

lexeme* evalSimpleOp(lexeme *tree, lexeme *env)
	{
	if ((*(*tree).left).type == PLUS) return evalPlus((*tree).right,env);
	if ((*(*tree).left).type == TIMES) return evalTimes((*tree).right,env);
	if ((*(*tree).left).type == DIVIDES) return evalDivides((*tree).right,env);
	if ((*(*tree).left).type == MINUS) return evalMinus((*tree).right,env);
	if ((*(*tree).left).type == MOD) return evalMod((*tree).right,env);
	if ((*(*tree).left).type == EXP) return evalExp((*tree).right,env);
	if ((*(*tree).left).type == EQUAL) return evalEqual((*tree).right,env); //this is not assignment
	if ((*(*tree).left).type == ASSIGN) 
		{
		evalAssign((*tree).right,env);
		return NULL;
		}
	if ((*(*tree).left).type == AND) return evalAnd((*tree).right,env);
	if ((*(*tree).left).type == OR) return evalOr((*tree).right,env);
	if ((*(*tree).left).type == GREATERTHAN) return evalGreaterThan((*tree).right,env);
	if ((*(*tree).left).type == LESSTHAN) return evalLessThan((*tree).right,env);
	else
		{
		return NULL;
		}
	}

lexeme* evalAnd(lexeme *tree, lexeme *env)
	{
	if ((*eval((*tree).left,env)).boolean == false)
		{
		return newBooleanLexeme(false);
		}
	else
		{
		return newBooleanLexeme((*eval((*tree).right,env)).boolean);
		}
	}

lexeme *evalOr(lexeme *tree, lexeme *env)
	{
	if ((*eval((*tree).left,env)).boolean == true)
		{
		return newBooleanLexeme(true);
		}
	else
		{
		return newBooleanLexeme((*eval((*tree).right,env)).boolean);
		}
	}

lexeme *evalGreaterThan(lexeme *tree, lexeme *env)
	{
	if ((*eval((*tree).left,env)).intvalue > (*eval((*tree).right,env)).intvalue)
		{
		return newBooleanLexeme(true);
		}
	else
		{
		return newBooleanLexeme(false);
		}
	}
lexeme *evalLessThan(lexeme *tree, lexeme *env)
	{
	if ((*eval((*tree).left,env)).intvalue < ((*eval((*tree).right,env)).intvalue))
		{
		return newBooleanLexeme(true);
		}
	else
		{
		return newBooleanLexeme(false);
		}
	}

		

lexeme* newNumberLexeme(int value)
	{
	lexeme *toReturn = new lexeme(NUMBER);
	//char *intStr = (value);
	//(*toReturn).String = string(intStr);
	(*toReturn).intvalue = value;
	return toReturn;
	}

lexeme* evalPlus(lexeme *tree,lexeme *env)
	{
	//eval the left and right sides
	lexeme *lval = eval((*tree).left,env);
	lexeme *rval = eval((*tree).right,env);
	if ((*lval).type == NUMBER && (*lval).type == NUMBER)
		{
		return newNumberLexeme((*lval).intvalue + (*rval).intvalue);
		}
	else if ((*lval).type == STRING && (*lval).type == STRING)
		{
		return newStringLexeme((*lval).String + (*rval).String);
		}
	else if ((*lval).type == BOOLEAN && (*lval).type == BOOLEAN)
		{
		return newBooleanLexeme(true);
		}
	else
		{
		return lval;
		}
	
	}

lexeme* evalTimes(lexeme *tree,lexeme *env)
	{
	lexeme *lval = eval((*tree).left,env);
	lexeme *rval = eval((*tree).right,env);
	if ((*lval).type == NUMBER && (*lval).type == NUMBER)
		{
		return newNumberLexeme((*lval).intvalue * (*rval).intvalue);
		}
	else if ((*lval).type == STRING && (*lval).type == STRING)
		{
		return newStringLexeme((*lval).String);
		}
	else if ((*lval).type == BOOLEAN && (*lval).type == BOOLEAN)
		{
		return newBooleanLexeme((*lval).boolean);
		}
	else
		{
		return lval;
		}
	}

lexeme* evalDivides(lexeme *tree,lexeme *env)
	{
	lexeme *lval = eval((*tree).left,env);
	lexeme *rval = eval((*tree).right,env);
	if ((*lval).type == NUMBER && (*lval).type == NUMBER)
		{
		return newNumberLexeme((*lval).intvalue / (*rval).intvalue);
		}
	else if ((*lval).type == STRING && (*lval).type == STRING)
		{
		return newStringLexeme((*lval).String);
		}
	else if ((*lval).type == BOOLEAN && (*lval).type == BOOLEAN)
		{
		return newBooleanLexeme((*lval).boolean);
		}
	else
		{
		return lval;
		}
	}

lexeme* evalMinus(lexeme *tree,lexeme *env)
	{
	lexeme *lval = eval((*tree).left,env);
	lexeme *rval = eval((*tree).right,env);
	if ((*lval).type == NUMBER && (*lval).type == NUMBER)
		{
		return newNumberLexeme((*lval).intvalue - (*rval).intvalue);
		}
	else if ((*lval).type == STRING && (*lval).type == STRING)
		{
		return newStringLexeme((*lval).String);
		}
	else if ((*lval).type == BOOLEAN && (*lval).type == BOOLEAN)
		{
		return newBooleanLexeme(false);
		}
	else
		{
		return lval;
		}
	}

lexeme* evalMod(lexeme *tree,lexeme *env)
	{
	lexeme *lval = eval((*tree).left,env);
	lexeme *rval = eval((*tree).right,env);
	if ((*lval).type == NUMBER && (*lval).type == NUMBER)
		{
		return newNumberLexeme((*lval).intvalue % (*rval).intvalue);
		}
	else if ((*lval).type == STRING && (*lval).type == STRING)
		{
		return newStringLexeme((*lval).String);
		}
	else if ((*lval).type == BOOLEAN && (*lval).type == BOOLEAN)
		{
		return newBooleanLexeme((*lval).boolean);
		}
	else
		{
		return lval;
		}
	}

lexeme* evalExp(lexeme *tree,lexeme *env)
	{
	lexeme *lval = eval((*tree).left,env);
	lexeme *rval = eval((*tree).right,env);
	if ((*lval).type == NUMBER && (*lval).type == NUMBER)
		{
		int lvalI = (*lval).intvalue;
		int rvalI = (*rval).intvalue;
		int exp = lvalI; 
		for (int i = 1; i < rvalI; ++i)
			{
			exp = exp * lvalI;
			}	
		return newNumberLexeme(exp);	
		}
	else if ((*lval).type == STRING && (*lval).type == STRING)
		{
		return newStringLexeme((*lval).String);
		}
	else if ((*lval).type == BOOLEAN && (*lval).type == BOOLEAN)
		{
		return newBooleanLexeme((*lval).boolean);
		}
	else
		{
		return lval;
		}
	}

lexeme* newBooleanLexeme(bool value)
	{
	lexeme *toReturn = new lexeme(BOOLEAN);
	(*toReturn).boolean = value;
	return toReturn;
	}

lexeme* evalEqual(lexeme *tree,lexeme *env)
	{
	lexeme *lval = eval((*tree).left,env);
	lexeme *rval = eval((*tree).right,env);
	if ((*lval).type == STRING && (*rval).type == STRING) //create error handling for string eq int
		{
		return newBooleanLexeme((*lval).String.compare((*rval).String) == 0);
		}
	else if ((*lval).type == NUMBER && (*rval).type == NUMBER)
		{
		return newBooleanLexeme((*lval).intvalue == (*rval).intvalue);	
		}
	else if ((*lval).type == BOOLEAN && (*rval).type == BOOLEAN)
		{
		return newBooleanLexeme((*lval).boolean == (*rval).boolean);
		}
	else if ((*lval).type == NIL && (*rval).type == NIL)
		{
		return newBooleanLexeme(true);
		}
	else
		{
		return newBooleanLexeme(lval == rval);
		}
			
	}


void evalAssign(lexeme *tree, lexeme *env)
	{
	//check book for how to deal with dot operators
	//eval only the right hand side
	if ((*(*tree).left).type != VARIABLE)
		{
		cerr << "SYNTAX ERROR! ASSIGNMENT LEFT OPERATOR NOT VARIABLE" << endl;
		exit(-1);
		}	
	lexeme *value = eval((*tree).right,env);
	updateEnv(env,(*tree).left,value);
	//displayEnv(env);
	}

	
void evalVarDef(lexeme *tree,lexeme *env)
	{
	if ((*(*tree).left).type != VARIABLE)
		{
		cerr << "SYNTAX ERROR! ASSIGNEMENT LEFT OPERATOR NOT VARIABLE" << endl;
		exit(-1);
		}
	lexeme *value = eval((*tree).right,env);
	insertEnv((*tree).left, value, env);
	}	

lexeme* evalIf(lexeme *tree, lexeme *env)
	{
	if ((*eval((*tree).left,env)).boolean == true)
		{
		return eval((*(*tree).right).left,env); //Block
		}
	else
		{
		return eval((*(*tree).right).right,env); //optElse Block
		}
	}
lexeme* evalWhile(lexeme *tree, lexeme *env)
	{
	while ((*eval((*tree).left,env)).boolean == true)
		{
		eval((*tree).right,env);
		}
	return NULL;
	}

void evalPrintHelper(int x)
	{
	cout << x;
	}
void evalPrintHelper(string s)
	{
	if (s.compare(">tab")== 0)
		{
		cout << "\t";
		}
	else if (s.compare(">line")==0)
		{
		cout << endl;
		}
	else
		{	
		cout << s;
		}
	}
void evalPrintHelper(bool s)
	{
	if (s)
		{
		cout << "true";
		}
	else
		{
		cout << "false";
		}
	}
lexeme *evalIsInt(lexeme *eargs)
	{
	if ((*car(eargs)).type == NUMBER)
		{
		return newBooleanLexeme(true);
		}
	else
		{
		return newBooleanLexeme(false);
		}
	}
lexeme *evalGetInput(lexeme *eargs)
	{
	if ((*car(eargs)).String.compare("std") == 0)
		{
		string s;
		if (!(cin >> s))
			{
			return newBooleanLexeme(false);
			}
		if ((s.compare("+")==0) || s.compare("*")==0 || s.compare("-")== 0|| s.compare("/")==0 || s.compare("^") == 0 || s.compare("=") == 0)
			{
			lexeme *toReturn = newStringLexeme(s);
			return toReturn;
			}
		else
			{
			lexeme *toReturn = newNumberLexeme(stoi(s.c_str()));
			return toReturn;
			}
		}
	else
		{
		cerr << "error invalid getIn source" << endl;
		exit(1);
		}
	return NULL;
	}
		 
lexeme *evalCreateArray(lexeme *eargs) 
	{
	if ((*car(eargs)).type != NUMBER || cdr(eargs) != NULL)
		{
		cerr << "error: invalid arguments in createArray function" << endl;
		exit(-3);
		}
	int size = (*car(eargs)).intvalue;
	if (size < 1)
		{
		cerr << "error: cannot declare array size less than 1" << endl;
		exit(-2);
		}
	lexeme *ar = new lexeme(ARRAY);
	(*ar).array = vector<lexeme*>(size,NULL);
	int i = 0;
	while (i < size)
		{
		(*ar).array[i] = new lexeme(UNDECLARED);
		++i;
		}
	//viewArray(array);
	return ar;
	}
void evalShowArray(lexeme *eargs)
	{
	lexeme *ar = car(eargs);
	cout << printG((*ar).type) << endl;
	lexeme *size = car(cdr(eargs));
	int s = (*size).intvalue;
	//cout << "size is" << s << endl;
	int i = 0;
	while (i < s)
		{
		(*(*ar).array[i]).display();
		++i;
		}
	}
void viewArray(lexeme *a) //used for testing
	{
	if ((*a).array[0] == NULL)
		{
		return;
		}
	else 
		{
	        cout << printG((*(*a).array[0]).type) << endl;
		}	
	}

lexeme *evalGetArray(lexeme *eargs)
	{
	lexeme *ar = car(eargs);
	//viewArray(ar);
	//(*ar).displayArray();
	lexeme *location = car(cdr(eargs));
	lexeme *shouldBeNull = (cdr(cdr(eargs)));
	if ((*ar).type != ARRAY || (*location).type != NUMBER || shouldBeNull != NULL)
		{
		cerr << "error: invalid arguments in getArray function" << endl;
		exit(1);
		}
	lexeme *toReturn = (*ar).array[(*location).intvalue];
	return toReturn;
	}
	
void evalSetArray(lexeme *eargs)
	{
	lexeme* ar = car(eargs);
	lexeme* location = car(cdr(eargs));
	lexeme* setVal = car(cdr(cdr(eargs)));
	//cout << "set array location at "<< (*location).intvalue << endl;
	//(*setVal).display();
	if ((*ar).type != ARRAY || (*location).type != NUMBER || (cdr(cdr(cdr(eargs)))) != NULL)
		{
		cerr << "error: invalid arguments in setArray function" << endl;
		exit(1);
		}
	(*ar).array[(*location).intvalue] = setVal;
	}	
lexeme *evalStrCompare(lexeme *eargs)
	{
	lexeme *str1 = car(eargs);
	lexeme *str2 = car(cdr(eargs));
	return newNumberLexeme((*str1).String.compare((*str2).String));
	}	
		
void evalPrint(lexeme *eargs)
	{
	while (eargs != NULL)
		{
		lexeme *check = car(eargs);
		if ((*check).type == NUMBER) {evalPrintHelper((*check).intvalue);}
		else if ((*check).type == STRING) {evalPrintHelper((*check).String);}
		else if ((*check).type == BOOLEAN) {evalPrintHelper((*check).boolean);}
		else
			{
		//	cerr << "not NUMBER, STRING, or BOOL" << endl;
			if (check == NULL)
				{
				cerr << "this lexeme is a null value" << endl;
				}
			else
				{
				cout << printG((*check).type); 
				}
			}
		eargs = cdr(eargs);
		}
	}

	
	
	
