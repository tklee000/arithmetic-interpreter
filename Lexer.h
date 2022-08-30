#pragma once
#include <string>
#include <vector>

using namespace std;

enum TokenType
{
    Integer,
    Plus,
    Minus,
    Mul,
    Div,
    LeftParens,
    RightParens,
    EndToken,
};

typedef struct _Token
{
    long integer;
    TokenType type;
} Token;

class Lexer
{
public :
    Lexer(string _text);
    ~Lexer();

    vector<Token> GetTokens();

private:
    string text;
    int textPos;
    int textLen;

    long GetInteger();
    void SkipSpace();
    Token GetNextToken();

};

