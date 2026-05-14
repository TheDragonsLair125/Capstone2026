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

    Stmt* declaration(){
        try{
            if(match({INT})){
                return varDecleration(NUMBER_VAL);
            }

            if(match({STRINGVAR})){
                return varDecleration(STRING_VAL);
            }

            return statement();
        }
        catch(const exception& e){
            return nullptr;
        }
    }

    Stmt* varDecleration(ValueType declaredType){
        if(peek().type != VAR){
            throw runtime_error("Expected name after deceleariton");
        }

        Token name = advance();
        Expr* initial = nullptr;

        if(match({EQUALS})){
        initial = expression();
        }
            
        if(!match({SEMICOLON})){
                    throw runtime_error("Expected ';' after expression.");
        }

        return new VarStmt(name, declaredType, initial);

    }

    Stmt* statement(){
        if(match({PRINT})){
            return printStatement();
        }
        if(match({CIN})){
            return InputStatement();
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

    Stmt* InputStatement(){
        if(!match({INPUT})){
                    throw std::runtime_error("Expected '>>' after cin.");
        }

        if(!match({VAR})){
                    throw std::runtime_error("Expected variable after >>.");
        }

        Token name = previous();

        if(!match({SEMICOLON})){
                    throw std::runtime_error("Expected ';' after expression.");
        }

        return new InputStmt(name);
    }

    Stmt* expressionStatement(){
        Expr* expr = expression();

        if(!match({SEMICOLON})){
                    throw runtime_error("Expected ';' after expression.");
        }
        
        return new ExprStmt(expr);
    }

    Expr* expression(){
        return assignment();
    }

    Expr* assignment(){
        Expr* expr = equality();
        
        if(match({EQUALS})){

            Expr* value = assignment();

            if(auto varExpr = dynamic_cast<Var*>(expr)){
                Token name = varExpr->var;
                return new Assign(name, value);
            }

            throw runtime_error("Invalid assignment target.");
        }

        return expr;
    }

    Expr* equality(){
        Expr* expr = comparison();

        while(match({EQUAL_TO, NOT_EQUAL_TO})){
            Token oper = previous();
            Expr* right = comparison();
            expr = new Math(expr, oper, right);
        }

        return expr;
    }

    Expr* comparison(){
        Expr* expr = as();

        while(match({GREATER_THAN, GREATER_THAN_EQUAL, LESS_THAN, LESS_THAN_EQUAL})){
            Token oper = previous();
            Expr* right = as();
            expr = new Math(expr, oper, right);
        }

        return expr;
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
        Expr* expr = unary();

        while(match({MULTIPLY, DIVIDE})){
            Token oper = previous();
            Expr* right = unary();
            //cout << tokens[current-1].value;
            //cout << tokens[current].value;
            expr = new Math(expr, oper, right);
        }

        return expr;
    }

    Expr* unary(){
        if(match({MINUS, NOT})){
            Token oper = previous();
            Expr* right = unary();
            return new Unary(oper, right);
        }

        return value();
    }

    Expr* value(){
        if(match({NUMBER,STRING})){
            //cout << tokens[current-1].value;
            return new Value(previous());
        }
        else if(match({VAR})){
            return new Var(previous());
        }

        if(match({LEFT_PAREN})){
            Expr* expr = expression();

            if(!match({RIGHT_PAREN})){
                    throw runtime_error("Expected ';' after expression.");
            }

            return new Group(expr);
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
                statements.push_back(declaration());
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