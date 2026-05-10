#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
//parser.h

#include "token.h"
#include <vector>
#include "expression.h"

using namespace std;

class Parser {
    private:
     vector<Token> tokens;
     int current = 0;

    Expr* expression(){
        return as();
    }

    Expr* as(){
        Expr* expr = md();

        while(match({PLUS, MINUS})){
            Token oper = previous();
            Expr* right = md();
            expr = new Math(expr, oper, right);
        }

        return expr;
    }

    Expr* md(){
        Expr* expr = value();

        while(match({MULTIPLY, DIVIDE})){
            Token oper = previous();
            Expr* right = value();
            expr = new Math(expr, oper, right);
        }

        return expr;
    }

    Expr* value(){
        if(match({NUMBER})){
            return new Value(previous());
        }
    }

    bool match(std::initializer_list<TokenType> types){

        for (TokenType type: types){
            if (check(type)) {
                advance();
                return true;
            }
        }

        return false;

    }

    bool check(TokenType type){

        if(isAtEnd()){ 
          return false;
        }

        return peek().type == type;
    }

    Token advance(){
        if(!isAtEnd()){
            current++;
        }

        return previous();
    }

    Token peek(){
        return tokens[current];
    }

    Token previous(){
        return tokens[current-1];
    }

    bool isAtEnd(){
        return peek().type == ENDFILE;
    }
};

#endif