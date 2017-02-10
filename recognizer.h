#ifndef RECOGNIZER_H
#define RECOGNIZER_H
using namespace std;

class recognizer
	{
	public:
	lexeme *CurrentLexeme;
	lexer *lexr;
	recognizer(lexeme*,lexer*);
	bool check(GTYPE type);
	lexeme* advance(void);
	lexeme* match(GTYPE);
	void matchNoAdvance(GTYPE);
	void printErrorMessage(GTYPE);
	lexeme* program(void);
	lexeme* programExprList(void);
	bool programPending(void);
	lexeme* unary(void);
	lexeme* expr(void);
	lexeme* operation(void);
	bool operationPending(void);
	bool logicalOperatorPending(void);
	lexeme* logicalOperator(void);
	lexeme* optExprList(void);	
	bool exprListPending(void);
	bool exprPending(void);
	bool unaryPending(void);
	lexeme* exprList(void);
	lexeme* ifStatement(void);
	bool ifStatementPending(void);
	lexeme* optElse(void);
	lexeme* block(void);
	bool blockPending(void);	
	lexeme* statements(void);
	bool statementsPending(void);
	lexeme* statement(void);
	bool statementPending(void);
	lexeme* whileLoop(void);
	bool whileLoopPending(void);
	lexeme* forLoop(void);
	bool forLoopPending(void);
	lexeme* varDef(void);
	bool varDefPending(void);
	lexeme* functionDef(void);
	bool functionDefPending(void);
	lexeme* functionCall(void);
	bool functionCallPending(void);
	lexeme* optParameterList(void);
	lexeme* parameterList(void);
	lexeme* parameterListHelper(void);
	bool parameterListPending(void);
	lexeme* runRecognizer(void);
	void runLexemeTest(void);
		};
#endif
	
