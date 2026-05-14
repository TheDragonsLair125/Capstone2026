//token.h

#ifndef TOKEN_H_INCLUDED_
#define TOKEN_H_INCLUDED_

#include<string>

enum TokenType{
    //single charcters
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    PLUS, MINUS, MULTIPLY, DIVIDE, SEMICOLON, EQUALS,
    NEGATIVE, NOT,

    //comparisons
    EQUAL_TO, NOT_EQUAL_TO, LESS_THAN, LESS_THAN_EQUAL, 
    GREATER_THAN, GREATER_THAN_EQUAL, TRUE, FALSE,

    //Variables
    VAR, INT, NIL, STRINGVAR, BOOL,

    //Others
    NUMBER, STRING, PRINT, OUTPUT, CIN, INPUT, ENDFILE
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