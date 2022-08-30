#include "Lexer.h"
#include <iostream>

Lexer::Lexer(string _text)
{
	text = _text;
	textLen = text.size();
	textPos = 0;
}

Lexer::~Lexer()
{

}

vector<Token> Lexer::GetTokens()
{
	vector<Token> tokens;
	Token token;
	while ((token = GetNextToken()).type != TokenType::EndToken)
	{
		tokens.push_back(token);
	}

	return tokens;
}

long Lexer::GetInteger()
{
	long integer = 0;
	char c = text.at(textPos);
	do
	{
		integer = integer * 10 + c - '0';
		textPos++;
		if (textPos == textLen)
			return integer;
		c = text.at(textPos);
	} while (c >= '0' && c <= '9');
	return integer;
}

void Lexer::SkipSpace()
{
	char CurrentChar = 0;
	do
	{
		textPos++;
		if (textPos == textLen)
			return;
		CurrentChar = text.at(textPos);
	} while (CurrentChar == ' ');
}

Token Lexer::GetNextToken()
{
	Token token;
	token.integer = 0;
	token.type = TokenType::EndToken;
	char c = 0;

	while (textPos < textLen)
	{
		c = text.at(textPos);

		// Get Interger Value From Token
		if (c >= '0' && c <= '9')
		{
			token.type = TokenType::Integer;
			token.integer = GetInteger();
			return token;
		}

		switch (c)
		{
			case ' ': SkipSpace(); continue;
			case '+': textPos++; token.type = TokenType::Plus;  return token;
			case '-': textPos++; token.type = TokenType::Minus;  return token;
			case '*': textPos++; token.type = TokenType::Mul;  return token;
			case '/': textPos++; token.type = TokenType::Div;  return token;
			case '(': textPos++; token.type = TokenType::LeftParens;  return token;
			case ')': textPos++; token.type = TokenType::RightParens;  return token;
		}

		if (c != 0)
		{
			std::cout << "Parse Token Error\n";
		}

	}

	return token;
}