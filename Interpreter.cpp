#include "Interpreter.h"
#include <iostream>


/* EBNF
factor = ( [ "+" | "-" ], integer ) | ( "(", expression, ")" )
term = factor, { "*" | "/", factor }
expression = term, { "+" | "-", term }
*/

Interpreter::Interpreter(vector<Token>* _tokens)
{
	tokens = _tokens;
	CurrentToken = 0;
}
Interpreter::~Interpreter()
{
}

long Interpreter::Run()
{
	return Expression();
}



long Interpreter::Factor()
{
	//	std::cout << "factor begin" << endl;

	Token token = GetCurrentToken();
	MoveNextToken();

	long factor;
	switch (token.type)
	{
	case TokenType::Plus:
	case TokenType::Minus:
		Token integer = GetCurrentToken();
		Match(TokenType::Integer);
		factor = integer.integer;
		if (token.type == TokenType::Minus)
			factor = -factor;
		break;

	case TokenType::Integer:
		factor = token.integer;
		break;

	case TokenType::LeftParens:
		long expression = Expression();
		Match(TokenType::RightParens);
		factor = expression;
		break;
	}

	//	std::cout << "factor end" << endl;

	return factor;

}


long Interpreter::Term()
{
	//	std::cout << "term begin" << endl;

	long term = Factor();

	Token token = GetCurrentToken();
	while (token.type == TokenType::Mul || token.type == TokenType::Div)
	{
		MoveNextToken();
		long factor = Factor();
		if (token.type == TokenType::Mul)
		{
			std::cout << to_string(term) << "*" << to_string(factor) << endl;
			term *= factor;
		}
		else
		{
			std::cout << to_string(term) << "/" << to_string(factor) << endl;
			term /= factor;
		}

		token = GetCurrentToken();
	}

	//std::cout << "term end" << endl;
	return term;
}


long Interpreter::Expression()
{
	//	std::cout << "expression begin" << endl;

	long expression = Term();

	Token token = GetCurrentToken();
	while (token.type == TokenType::Plus || token.type == TokenType::Minus)
	{
		MoveNextToken();
		long term = Term();
		if (token.type == TokenType::Plus)
		{
			std::cout << to_string(expression) << "+" << to_string(term) << endl;
			expression += term;
		}
		else
		{
			std::cout << to_string(expression) << "-" << to_string(term) << endl;
			expression -= term;
		}

		token = GetCurrentToken();
	}

	//	std::cout << "expression end" << endl;

	return expression;
}


Token Interpreter::GetCurrentToken() {

	if (CurrentToken < tokens->size())
		return tokens->at(CurrentToken);

	Token token;
	token.integer = 0;
	token.type = TokenType::EndToken;
	return token;
};

void Interpreter::Match(TokenType type)
{
	if (GetCurrentToken().type != type) {
		std::cout << "Interpreter Match Error\n";
	}
	MoveNextToken();
}