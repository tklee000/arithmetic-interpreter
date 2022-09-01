#include <iostream>
#include <string>
#include "Lexer.h"
#include "Interpreter.h"

using namespace std;

int main()
{
    string input = "2*(3+1)";

    Lexer lexer(input);
    vector<Token> Tokens = lexer.GetTokens();
    Interpreter interpreter(&Tokens);
    
    for (int i = 0; i < Tokens.size(); i++)
    {
        switch (Tokens[i].type)
        {
            case TokenType::Integer:
            {
                std::cout << to_string(Tokens[i].integer);
                break;
            }
            case TokenType::Plus:
            {
                std::cout << "+";
                break;
            }
            case TokenType::Minus:
            {
                std::cout << "-";
                break;
            }
            case TokenType::Mul:
            {
                std::cout << "*";
                break;
            }
            case TokenType::Div:
            {
                std::cout << "/";
                break;
            }
            case TokenType::LeftParens:
            {
                std::cout << "(";
                break;
            }
            case TokenType::RightParens:
            {
                std::cout << ")";
                break;
            }
        }
    }
    
    std::cout << endl;
    std::cout << "=" << interpreter.Run() << endl;
    
    return 0;
}
