#pragma once

#include "Lexer.h"

class Interpreter
{
public :
	Interpreter(vector<Token> *_tokens);
	~Interpreter();
	long Run();

private:
	vector<Token> *tokens;
	int CurrentToken;
	void MoveNextToken() { CurrentToken++; }
	Token GetCurrentToken();
	void Match(TokenType type);
	long Factor();
	long Term();
	long Expression();
	
	
};

