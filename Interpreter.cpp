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
term�� factor �� ����� �ٷ� �ɼ��� �ִ�. Ȥ�� ���� ��ū�� (*,/) �ΰ�� ���� factor �� �����Ѵ�.
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
  expression�� term �� ����� �ٷ� �ɼ��� �ִ�. Ȥ�� ���� ��ū�� +- �ΰ�� ���� term �� �����Ѵ�.

  ��1) 1*3 �ΰ��� term �� �ٷ� expression �̵Ǹ�, expression ������ ������ ��ŵ

  ��2) 1+2 �ΰ�쿡�� term �� Call ������ term ������ ������ ��ŵ�ǰ� expression = factor�� �ȴ�...

  ��3) 1+2*3 �� ��� 2*3 �� ���� ����Ǵ� ���� ����

       1 + ���� ó����
	   1 + X   --> X term�� Call (while ������)
	   term�� 2*3 �� ó���� return

	   expression �Լ��� �Ϸ���� ����ä �߰����� Call�� �߱� ������ 
	   term ����� ��ٸ��鼭 expression �� �����ȴ�. 
	   expression �� ó���ϴ� �߰� term �Լ�ȣ������ ���ؼ�
	   term �Լ� ���� ������ ���� ó���Ǿ ���� ������ �����ȴ�.
	   �߰� �Լ� ȣ��� ���� ������ �� �켱������ �ٲٴ� ����� �ȴ�.

	   ����� EBNF �������� ����켱 ������ �ִ� ��� ���� ���� �������� ó�������ϴ�.
	   term = factor, { "*" | "/", factor }
	   expression = term, { "+" | "-", term }
	   term, expression �� ���� ó�� �Լ��� ����� ó���ؾ� �ϸ�, �Լ� Call �������� �ۼ��ؾ� �Ѵ�.

   ��4) 2*(3+1) �� ��� (3+1) �� ���� ����Ǵ� ���� ����

	   2 * ���� ó����
	   2 * X   --> X factor�� Call (term while ������)
	   factor�� ( �� ������ ���ο� expression �� ó���ϵ��� �Լ� call expression
	   term �Լ��� �Ϸ���� ����ä call factor 
	   factor ���� �Լ��� �Ϸ���� ����ä call expression
	   �ι� ���� �Լ� Call �Ǵ� ��Ȳ��
	   factor���� call�� expression �� �Ϸ�ǰ� factor�� ���� �Ϸ�Ǳ� �������� term �� ����~~
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