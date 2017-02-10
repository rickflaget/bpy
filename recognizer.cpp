#include "grammartypes.h"
#include "ENV.h"
#include "evaluate.h"
#include "lexer.h"
#include <stdlib.h>
using namespace std;

class recognizer
	{
	public:
	lexeme *CurrentLexeme;
	lexer *lexr;
	recognizer(lexeme *c, lexer* g)
		{
		CurrentLexeme = c;
		lexr = g;
		};

	bool check(GTYPE type)
		{
		if ((*CurrentLexeme).type == COMMENT)
			{
			advance();
			}
		return ((*CurrentLexeme).type == type);
		}

	lexeme* advance(void)
		{
		lexeme *olexeme = CurrentLexeme;
		CurrentLexeme = (*lexr).lex();
	//	(*CurrentLexeme).display();// show input: useful for error tracking
		return olexeme;
		}

	lexeme* match(GTYPE type)
		{
		matchNoAdvance(type);
		return advance();
		}

	void matchNoAdvance(GTYPE type)
		{
		if (!check(type))
			{
			printErrorMessage(type);
			}
		}
	void printErrorMessage(GTYPE type)
		{
		cerr << "synatx error: ";
		cerr << printG((*CurrentLexeme).type);
		cerr << " not of expected type: ";
		cerr << printG(type) << endl;
		exit(1);
		}

	lexeme* program(void)
		{
		lexeme *tree = new lexeme(PROGRAM);
		(*tree).left = programExprList();
		if (programPending())
			{
			(*tree).right = program();
			}
		return tree;
		}
	lexeme* programExprList(void)
		{
		lexeme *tree;
		if (functionDefPending())
			{
			tree = functionDef();
			}
		else if (varDefPending())
			{
			tree = varDef();
			}
		else if (functionCallPending())
			{
			tree = functionCall();
			}
		else
			{
			cerr << "Synatx Error: expected a FunctionDefintion, variable Definintion, or a Function Call!" << endl;
			exit(1);
			}
		return tree;
		}

		bool programPending(void)
			{
			return functionDefPending() || varDefPending() || functionCallPending();
			}
	lexeme* unary(void)
		{
		lexeme* tree;
		if (check(NUMBER))
			{
			tree = advance();
			}
		else if (check(BOOLEAN))
			{
			tree = advance();
			}
		else if (check(NIL))
			{
			tree = advance();
			}
		else if (check(STRING))
			{
			tree = advance();
			}
		else if (check(ARRAY))
			{
			tree = advance();
			}
		else if (check(VARIABLE))
			{
			//two cases! 
			lexeme *u = match(VARIABLE);
			if (check(OPAREN))
				{
				tree = new lexeme(FUNCTIONCALL);
				(*tree).left = u;
				advance(); //match(OPAREN)
				(*tree).right = optExprList();
				match(CPAREN);
				}
			else
				{
				tree = u;
				} 
			}
		else if (check(MINUS))
			{
			advance(); //match(MINUS)
			tree = new lexeme(UMINUS);
			(*tree).left = NULL;
			(*tree).right = unary();
			}
		else if (check(NOT))
			{
			advance();
			tree = new lexeme(NOT);
			(*tree).left = NULL;
			(*tree).right = unary();
			}
		else if (check(OPAREN))
			{
			tree = advance(); //match(OPAREN)
			(*tree).left = NULL;
			(*tree).right = expr();
			match(CPAREN); //leave this in???
			}
		else if (functionDefPending())
			{
			tree = functionDef();
			}
		else
			{
			tree = NULL;
			printErrorMessage(UNARY);
			return NULL;
			}
		return tree;
		}

	lexeme* expr(void)
		{
		lexeme *u = unary();
		if (operationPending())
			{
			lexeme *tree = new lexeme(BINARY);
			(*tree).left = operation();
			(*tree).right = new lexeme(GLUE);
			(*(*tree).right).left = u;
			(*(*tree).right).right = expr();
			return tree;
			}
		else
			{
			return u;
			}
		}

	lexeme* operation(void)
		{
		lexeme* tree;
		if (check(PLUS))
			{
			tree = advance();
			}
		else if (check(TIMES))
			{
			tree = advance();
			}
		else if (check(DIVIDES))
			{
			tree = advance();
			}
		else if (check(MINUS))
			{
			tree = advance();
			}
		else if (check(MOD))
			{
			tree = advance();
			}
		else if (check(EXP))
			{
			tree = advance();
			}
		else if (check(ASSIGN))
			{
			tree = advance();
			}
		else if (logicalOperatorPending())
			{
			tree = logicalOperator();
			}
		else
			{
			tree = NULL;
			printErrorMessage(OPERATOR);
			}
		return tree;
		}

	bool operationPending(void)
		{
		return check(PLUS) || check(TIMES) || check(DIVIDES) || check(MINUS) || check(MOD) || check(EXP) || check(ASSIGN) || logicalOperatorPending();
		}

	bool logicalOperatorPending(void)
		{
		return check(EQUAL) || check(GREATERTHAN) || check(LESSTHAN) || check(AND) || check(OR);
		}

	lexeme* logicalOperator(void)
		{
		lexeme* tree;
		if (check(EQUAL))
			{
			tree = advance();
			}
		else if (check(GREATERTHAN))
			{
			tree = advance();
			}
		else if (check(LESSTHAN))
			{
			tree = advance();
			}
		else if (check(AND))
			{
			tree = advance();
			}
		else if (check(OR))
			{
			tree = advance();
			}
		else
			{
			tree = NULL;
			printErrorMessage(LOGICALOPERATOR);
			}
		return tree;
		}

	lexeme* optExprList(void)
		{
		if (exprListPending())
			{
			return exprList();
			}
		else
			{
			return NULL;
			}
		}

	bool exprListPending(void)
		{
		return exprPending() || functionDefPending();
		}

	bool exprPending(void)
		{
		return unaryPending();
		}

	bool unaryPending(void)
		{
		return check(NIL) || check(NOT) || check(NUMBER) || check(ARRAY) || check(BOOLEAN) || check(STRING) || check(VARIABLE) || check(OPAREN) || check(MINUS);
		}

	lexeme* exprList(void)
		{
		lexeme* u;
		if (exprPending())
			{
			u = expr();
			if (check(COMMA))
				{
				lexeme* tree = new lexeme(LIST);
				(*tree).left = u;
				advance();//match(COMMA)
				(*tree).right = exprList();
				return tree;
				}
			else
				{
				return u;
				}
			}
		else if (check(ANON))
			{
			u = functionDef();
			if (check(COMMA))
				{
				lexeme* tree = new lexeme(LIST);
				(*tree).left = u;
				advance();//match(COMMA)
				(*tree).right = exprList();
				return tree;
				}
			else
				{
				return u;
				}
			}
		return NULL;
		}

	lexeme* ifStatement(void)
		{
		lexeme* tree;
		tree = match(IF);
		match(OPAREN);
		(*tree).left = expr();
		match(CPAREN);
		(*tree).right = new lexeme(GLUE);
		(*(*tree).right).left = block();
		(*(*tree).right).right = optElse();
		return tree;
		}

	bool ifStatementPending(void)
		{
		return check(IF);
		}

	lexeme* optElse(void)
		{
		lexeme *tree;
		if (check(ELSE))
			{
			tree = advance(); //match(ELSE)
			if (blockPending())
				{
				(*tree).left = NULL;
				(*tree).right = block();
				return tree;
				}
			else if (ifStatementPending())
				{
				(*tree).left = NULL;
				(*tree).right = ifStatement();
				return tree;
				}
			else
				{
				cerr << "Opt Else Syntax Error!" << endl;
				exit(1);
				}
			}
		else
			{
			return NULL;
			}
		}

	lexeme* block(void)
		{
		lexeme * tree;
		tree = match(OBRACE);
		(*tree).left = NULL;
		(*tree).right = statements();
		match(CBRACE);
		return tree;
		}

	bool blockPending(void)
		{
		return check(OBRACE);
		}

	lexeme* statements(void)
		{
		lexeme *u;
		lexeme *tree;
		u = statement();
		if (statementsPending())
			{
			tree = new lexeme(STATEMENTS);
			(*tree).left = u;
			(*tree).right = statements();
			return tree;
			}
		else
			{
			return u;
			}
		}

	bool statementsPending(void)
		{
		return statementPending();
		}

	lexeme* statement(void)
		{
		lexeme * tree;
		if (exprPending())
			{
			tree = expr();
			}
		else if (ifStatementPending())
			{
			tree = ifStatement();
			}
		else if (whileLoopPending())
			{
			tree = whileLoop();
			}
		else if (forLoopPending())
			{
			tree = forLoop();
			}
		else if (varDefPending())
			{
			tree = varDef();
			}
		else if (functionCallPending())
			{
			tree = functionCall();
			}
		else if (functionDefPending())
			{
			tree = functionDef();
			}
		else
			{
			tree = NULL;
			printErrorMessage(STATEMENT);
			}
		return tree;
		}

	bool statementPending(void)
		{
		return exprPending() || ifStatementPending() || whileLoopPending() || forLoopPending() || varDefPending() || functionCallPending() || functionDefPending();
		}

	lexeme* whileLoop(void)
		{
		lexeme* tree;
		tree = match(WHILE);
		match(OPAREN);
		(*tree).left = expr();
		match(CPAREN);
		(*tree).right = block();
		return tree;
		}

	bool whileLoopPending(void)
		{
		return check(WHILE);
		}

	lexeme* forLoop(void)
		{
		lexeme* tree;
		tree = match(FOR);
		match(OPAREN);
		(*tree).left = new lexeme(GLUE);
		(*(*tree).left).left= match(VARIABLE);
		(*(*tree).left).right = new lexeme(GLUE);
		(*(*(*tree).left).right).left = expr();
		(*(*(*tree).left).right).right = expr();
		match(CPAREN);
		(*tree).right= block(); 
		return tree;
		}

	bool forLoopPending(void)
		{
		return check(FOR);
		}

	lexeme* varDef(void)
		{
		lexeme *tree;
		tree = match(VAR);
		(*tree).left = match(VARIABLE);
		(*tree).right = NULL;
		if (check(ASSIGN))
			{
			advance(); //match(ASSIGN)
			if (exprPending())
				{
				(*tree).right = expr();
				}
			else if (functionDefPending())
				{
				(*tree).right = expr();
				}
			else
				{
				cerr << "Synatx error: right operand of assignment is not expr or funcDef" << endl;
				exit(1);
				}

			}
		return tree;
		}

	bool varDefPending(void)
		{
		return check(VAR);
		}

	lexeme* functionDef(void)
		{
		lexeme *tree;
		if (check(ANON))
			{
			tree = match(ANON);
			match(OPAREN);
			lexeme *rl = optParameterList();
			match(CPAREN);
			lexeme *rr = block();
			if (check(OBRACKET))
				{
				tree = new lexeme(ANONCALL);
				advance();
				(*tree).left = optExprList();
				(*tree).right = new lexeme(GLUE);
				(*(*tree).right).left= rl;
				(*(*tree).right).right= rr;
				match(CBRACKET);
				}
			else
				{
				(*tree).left = NULL;
				(*tree).right = new lexeme(GLUE);
				(*(*tree).right).left= rl;
				(*(*tree).right).right= rr;
				}
			return tree;
			}
		tree = match(FUNCTION);
		(*tree).left = match(VARIABLE);
		match(OPAREN);
		(*tree).right =new lexeme(GLUE);
		(*(*tree).right).left = optParameterList();
		match(CPAREN);
		(*(*tree).right).right = block();
		return tree;
		}

	bool functionDefPending(void)
		{
		return check(FUNCTION) || check(ANON);
		}

	lexeme* functionCall(void)
		{
		lexeme *tree = new lexeme(FUNCTIONCALL);
		(*tree).left = match(VARIABLE);
		match(OPAREN);
		(*tree).right = optExprList();
		match(CPAREN);
		return tree;
		}

	bool functionCallPending(void)
		{
		return check(VARIABLE);
		}

	lexeme* optParameterList(void)
		{
		if (parameterListPending())
			{
			return parameterList();
			}
		else
			{	
			return NULL;
			}
		}	


	lexeme* parameterList(void)
		{
		lexeme* treetop = new lexeme(OPAREN);
		(*treetop).left = NULL;
		(*treetop).right = parameterListHelper();
		return treetop;
		}

	lexeme* parameterListHelper(void)
		{
		lexeme* u = match(VARIABLE);
		lexeme* tree = new lexeme(LIST);
		//(*tree).left = match(VARIABLE);
		//(*tree).right = NULL;
		if (check(COMMA))
			{
			advance(); //match(COMMA)
			(*tree).left = u;
			(*tree).right = parameterListHelper();
			return tree;
			}
		return u;
		}


	bool parameterListPending(void)
		{
		return check(VARIABLE);
		}

	lexeme* runRecognizer(bool x)
		{
		lexeme *tree;
		advance();
		tree = program();
		advance();
		if ((*CurrentLexeme).type !=ENDofINPUT)
			{
			printErrorMessage(ENDofINPUT);
			}
		if (x == true)
			{
			cout << "running pretty printer" << endl <<  endl;
			prettyPrint(tree);
			cout << endl << endl << "end of pretty printer" << endl;
			}
		return tree;
		}
	void runLexemeTest(void) //not called
		{
		lexeme *test;
		test = advance();
		test = expr();
		advance();
		if ((*CurrentLexeme).type == ENDofINPUT)
			{
			cout << "correct!" << endl;
			}
		else
			{
			cout << "error!" << endl;
			}
		prettyPrint(test);
		}
	void prettyPrint(lexeme *tree)
		{
		if (tree == NULL)
			{
			return;
			}
		//(*tree).display();
		switch ((*tree).type)
			{
			case NUMBER: {cout <<(*tree).intvalue; break;} //change to int later
			case VARIABLE: {cout << (*tree).String; break;}
			case STRING: {cout << "\""  << (*tree).String << "\""; break;}
			case BOOLEAN: {cout << (*tree).String; break;}
			case NIL: {cout << "nil"; break;}
			case OPAREN:
				{
				cout << "(";
				prettyPrint((*tree).right);
				cout << ") ";
				break;
				}
			case UMINUS:
				{
				cout << "-";
				prettyPrint((*tree).right);
				break;
				}
			case NOT:
				{
				cout << "!";
				prettyPrint((*tree).right);
				break;
				}
			case BINARY: //expr op expr
				{
				prettyPrint((*(*tree).right).left); //the left operand
				cout << printINPUT((*(*tree).left).type);
				prettyPrint((*(*tree).right).right); //the right operand
				break;
				}
			case LIST:
				{
				prettyPrint((*tree).left);
				if ((*tree).right != NULL)
					{
					cout << ", ";
					prettyPrint((*tree).right);
					}
				break;
				}
			case ELSE:
				{
				cout << "else ";
				//left is null
				prettyPrint((*tree).right);
				break;
				}
			case ANON:
				{
				cout << "anon ";
				prettyPrint((*(*tree).right).left);
				prettyPrint((*(*tree).right).right);
				if ((*tree).left != NULL)
					{
					cout << "(";
					prettyPrint((*tree).left);
					cout << ")";
					}
				else
					{
					prettyPrint((*tree).left);
					}
				break;
				}
			case ANONCALL:
				{
				cout << "anon " ;
				prettyPrint((*(*tree).right).left);
				prettyPrint((*(*tree).right).right);
				cout << "[";
				prettyPrint((*tree).left);
				cout << "]";
				break;
				}
			case FUNCTION:
				{
				cout << "function ";
				prettyPrint((*tree).left);
				if ((*(*tree).right).left == NULL)
					{
					cout << "( )" << endl;
					}
				prettyPrint((*tree).right);
				cout << endl;
				break;
				}
			case OBRACE: //block
				{
				cout << "{ ";
				//left is null
				prettyPrint((*tree).right);
				cout << "} ";
				break;
				}
			case STATEMENTS:
				{
				cout << " ";
				prettyPrint((*tree).left);
				prettyPrint((*tree).right);
				break;
				}
			case WHILE:
				{
				cout << "while (";
				prettyPrint((*tree).left);//expr()
				cout << ") ";
				prettyPrint((*tree).right); //block()
				break;
				}
			case FOR:
				{
				cout << "for (";
				prettyPrint((*tree).left); //glue L: var R: GLUE 
				cout << ") ";
				prettyPrint((*tree).right); //block
				break;
				}
			case GLUE:
				{
				prettyPrint((*tree).left);
				prettyPrint((*tree).right);
				break;
				}
			case VAR:
				{
				cout << "var ";
				prettyPrint((*tree).left);//variable value
				if ((*tree).right != NULL)
					{
					cout << " = ";
					prettyPrint((*tree).right); //expr()
					}
				cout << " ";
				break;
				}
			case FUNCTIONCALL:
				{
				prettyPrint((*tree).left);
				cout << "(";
				prettyPrint((*tree).right);
				cout << ") ";
				break;
				}
			case PROGRAM:
				{
				prettyPrint((*tree).left);
				prettyPrint((*tree).right);
				break;
				}
			case IF:
				{
				cout << "\nif (";
				prettyPrint((*tree).left);
				cout << ")\n\t";
				prettyPrint((*tree).right);
				break;
				}
			case COMMENT:
				{
				cout << "IS THIS THE ISSUE" << endl;
				break;
				}
			default:
				{
				cerr << "Parse tree not recognized" << endl;
				exit(1);
				}
			}
		}

	};

int main(int argc, char *argv[])
	{
	if (argc > 2)
		{
		lexeme *dictToken = new lexeme(UNKNOWN);
		char *dictInput = argv[2];
		lexer *dictLexr = new lexer(dictInput);
		recognizer *dictRecog = new recognizer(dictToken, dictLexr);
		lexeme *dictTree = (*dictRecog).runRecognizer(false); //does not run pretty printer	

		lexeme *token = new lexeme(UNKNOWN);
		char * input = argv[1];
		lexer *lexr = new lexer(input);
		recognizer *recog = new recognizer(token, lexr);
		lexeme *tree = (*recog).runRecognizer(true); //does run pretty printer
		cout << endl;
		lexeme *env = createEnv();
		lexeme *originalTree = dictTree;
		while ((*dictTree).right != NULL)
			{
			dictTree = (*dictTree).right; //adds dict functions to front of tree
			}
		(*dictTree).right = tree;
		eval(originalTree,env);
		}
	else
		{
		lexeme *token = new lexeme(UNKNOWN);
		char * input = argv[1];
		lexer *lexr = new lexer(input);
		recognizer *recog = new recognizer(token, lexr);
		lexeme *tree = (*recog).runRecognizer(true); //does run pretty printer
		cout << endl;
		lexeme *env = createEnv();
		eval(tree,env);
		}
	return 0;
	}

