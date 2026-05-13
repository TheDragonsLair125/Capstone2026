#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
//parser.h

#include "token.h"
#include <vector>
#include "expression.h"
#include <stdexcept>
#include <iostream>

using namespace std;

class Parser {
    private:
     vector<Token> tokens;
     int current = 0;

    Stmt* statement(){
        if(match({PRINT})){
            return printStatement();
        }

        return expressionStatement();
    }

    Stmt* printStatement(){

        if(!match({OUTPUT})){
                    throw runtime_error("Expected '<<' after cout.");
        }

        Expr* expr = expression();

        if(!match({SEMICOLON})){
                    throw runtime_error("Expected ';' after expression.");
        }

        return new PrintStmt(expr);
    }

    Stmt* expressionStatement(){
        Expr* expr = expression();

        if(!match({SEMICOLON})){
                    throw runtime_error("Expected ';' after expression.");
        }
        
        return new ExprStmt(expr);
    }

    Expr* expression(){
        return as();
    }

    Expr* as(){
        Expr* expr = md();

        while(match({PLUS, MINUS})){
            Token oper = previous();
            Expr* right = md();
            //cout << tokens[current-1].value;
            //cout << tokens[current].value;
            expr = new Math(expr, oper, right);
        }

        return expr;
    }

    Expr* md(){
        Expr* expr = value();

        while(match({MULTIPLY, DIVIDE})){
            Token oper = previous();
            Expr* right = value();
            //cout << tokens[current-1].value;
            //cout << tokens[current].value;
            expr = new Math(expr, oper, right);
        }

        return expr;
    }

    Expr* value(){
        if(match({NUMBER})){
            //cout << tokens[current-1].value;
            return new Value(previous());
        }
        
        throw runtime_error("Expected Value.");
    }

    bool match(initializer_list<TokenType> types){

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

    

public:
     Parser(vector<Token> tokens)
    :tokens(tokens){}

    vector<Stmt*> parse(){

        vector<Stmt*> statements;

        try{
            while(!isAtEnd()){
                statements.push_back(statement());
            }
        }
        catch(const exception& e){
            cout << e.what() << endl;
        }       

        return statements;
    }

    void printExpr(Expr* expr) {

    if (auto val = dynamic_cast<Value*>(expr)) {
        cout << val->value.value;
    }

    else if (auto math = dynamic_cast<Math*>(expr)) {

        cout << "(";

        printExpr(math->left);

        cout << " " << math->oper.value << " ";

        printExpr(math->right);

        cout << ")";
    }
    }

};

#endif