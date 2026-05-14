//token.h

#ifndef TOKEN_H_INCLUDED_
#define TOKEN_H_INCLUDED_

#include<string>

enum TokenType{
    //single charcters
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    PLUS, MINUS, MULTIPLY, DIVIDE, SEMICOLON, EQUALS,

    //Others
    VAR, INT, NIL, NUMBER, STRING, STRINGVAR, PRINT, OUTPUT, CIN, INPUT, ENDFILE
};

class Token{
    public:
        TokenType type;
        std::string value;
        int line;
    
    Token(TokenType t, std::string v, int l){
        type = t;
        value = v;
        line = l;
    }
        
};

#endif