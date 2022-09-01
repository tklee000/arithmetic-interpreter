#include "Interpreter.h"
#include <iostream>


/* EBNF
factor = ( [ "+" | "-" ], integer ) | ( "(", expression, ")" )
term = factor, { "*" | "/", factor }
expression = term, { "+" | "-", term }
*/

int depth = 0;
void display_indent(string title)
{
	for (int i = 0; i < depth; i++) std::cout << " ";
	std::cout << title << endl;
}

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


// factor = ( [ "+" | "-" ], integer ) | ( "(", expression, ")" )

long Interpreter::Factor()
{
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

	depth++;
	display_indent("factor");
	depth--;
	return factor;

}

/*
term = factor, { "*" | "/", factor }
term은 factor 의 결과가 바로 될수도 있다. 혹은 다음 토큰이 (*,/) 인경우 다음 factor 과 연산한다.
*/



long Interpreter::Term()
{
	
	depth++;
	display_indent("term begin");
	long term = Factor();

	Token token = GetCurrentToken();
	while (token.type == TokenType::Mul || token.type == TokenType::Div)
	{
		

		MoveNextToken();
		long factor = Factor();

		
		if (token.type == TokenType::Mul)
		{
			string ret = to_string(term) + "*" + to_string(factor);
			display_indent(ret);
			term *= factor;
		}
		else
		{
			string ret = to_string(term) + "/" + to_string(factor);
			display_indent(ret);
			term /= factor;
		}

		token = GetCurrentToken();

		
	}

	display_indent("term end");
	depth--;
	
	return term;
}

/*
  expression = term, { "+" | "-", term }
  expression은 term 의 결과가 바로 될수도 있다. 혹은 다음 토큰이 +- 인경우 다음 term 과 연산한다.

  예1) 1*3 인경우는 term 이 바로 expression 이되며, expression 내부의 로직은 스킵

  예2) 1+2 인경우에도 term 을 Call 하지만 term 내부의 로직은 스킵되고 expression = factor로 된다...

  예3) 1+2*3 인 경우 2*3 이 먼저 연산되는 과정 설명

       1 + 까지 처리후
	   1 + X   --> X term을 Call (while 루프안)
	   term은 2*3 을 처리후 return

	   expression 함수가 완료되지 않은채 중간에서 Call을 했기 때문에 
	   term 결과를 기다리면서 expression 은 보류된다. 
	   expression 을 처리하다 중간 term 함수호출으로 인해서
	   term 함수 안의 연산이 먼저 처리되어서 현재 연산이 보류된다.
	   중간 함수 호출로 연산 보류가 즉 우선순위를 바꾸는 결과가 된다.

	   참고로 EBNF 문법에서 연산우선 순위가 있는 경우 위와 같은 문법으로 처리가능하다.
	   term = factor, { "*" | "/", factor }
	   expression = term, { "+" | "-", term }
	   term, expression 은 각각 처리 함수를 만들어 처리해야 하며, 함수 Call 형식으로 작성해야 한다.

   예4) 2*(3+1) 인 경우 (3+1) 이 먼저 연산되는 과정 설명

	   2 * 까지 처리후
	   2 * X   --> X factor을 Call (term while 루프안)
	   factor는 ( 를 만나서 새로운 expression 을 처리하도록 함수 call expression
	   term 함수가 완료되지 않은채 call factor 
	   factor 또한 함수가 완료되지 않은채 call expression
	   두번 연속 함수 Call 되는 상황임
	   factor에서 call한 expression 이 완료되고 factor로 전부 완료되기 전까지는 term 도 보류~~
*/


long Interpreter::Expression()
{
	depth++;
	display_indent("expression begin");
	long expression = Term();

	Token token = GetCurrentToken();
	while (token.type == TokenType::Plus || token.type == TokenType::Minus)
	{
		

		MoveNextToken();
		long term = Term();

		if (token.type == TokenType::Plus)
		{
			string ret = to_string(expression) + "+" + to_string(term);
			display_indent(ret);
			expression += term;
		}
		else
		{
			string ret = to_string(expression) + "-" + to_string(term);
			display_indent(ret);

			expression -= term;
		}

		token = GetCurrentToken();
		
	}

	display_indent("expression end");
	depth--;

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