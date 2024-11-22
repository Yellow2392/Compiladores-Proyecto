#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
    enum Type {
        PLUS, MINUS, MUL, DIV, NUM, ERR, PD, PI, END, ID, PRINT, 
        ASSIGN, TWODOT, DOTS, PC, LT, LE, EQ, NEQ, GT, GE, IF, ELSE, WHILE, DO,
        COMA, VAR, VAL, FOR, TRUE, FALSE, RETURN, FUN, LLD, LLI, IN
    };

    Type type;
    std::string text;

    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const std::string& source, int first, int last);

    friend std::ostream& operator<<(std::ostream& outs, const Token& tok);
    friend std::ostream& operator<<(std::ostream& outs, const Token* tok);
};

#endif // TOKEN_H