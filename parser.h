#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
//parser.h does the parsing of the tokens based on user input

#include "token.h"
#include <vector>
#include "expression.h"
#include <stdexcept>
#include <iostream>

using namespace std;

class Parser {
    private:
    //variables current used to track location in vector
     vector<Token> tokens;
     int current = 0;

    //highest precedent of the AST with variable decleration
    Stmt* declaration(){
        //Checks variable type before creating one of the correct value
            if(match({INT})){
                return varDecleration(NUMBER_VAL);
            }

            if(match({STRINGVAR})){
                return varDecleration(STRING_VAL);
            }

            if(match({BOOL})){
                return varDecleration(BOOL_VAL);
            }

            return statement();
        
    }

    //variabkle decleration
    Stmt* varDecleration(ValueType declaredType){
        //makes sure variable is created propery
        if(peek().type != VAR){
            throw runtime_error("Expected name after declaration, at line" + tokens[current].line);
        }

        Token name = advance();
        Expr* initial = nullptr;

        //checks to see if variable starts inialized or not
        if(match({EQUALS})){
        initial = expression();
        }
            
        //makes sure statement follows grammer and ends with ;
        if(!match({SEMICOLON})){
                    throw runtime_error("Expected ';' after expression, at line" + tokens[current-1].line);
        }

        //creates new VarStmt object with right info
        return new VarStmt(name, declaredType, initial);

    }

    //Checks what type of token is used to see what type of statement to run
    Stmt* statement(){

        if(match({PRINT})){
            return printStatement();
        }
        if(match({CIN})){
            return InputStatement();
        }
        if(match({IF})){
            return IfStatement();
        }
        //next two prevent errors as in C++ you cant have an else statement by itself or block of code without an error.
        if(match({ELSE})){
            throw runtime_error("Expected a statement, at line" + tokens[current].line);
        }
        
        if(match({LEFT_BRACE})){
            throw runtime_error("Expected a statement, at line" + tokens[current].line);
        }
        if(match({WHILE})){
            return whileStatement();
        }

        return expressionStatement();
    }

    //Print statements
    Stmt* printStatement(){

        //done to make sure proper format is done
        if(!match({OUTPUT})){
                    throw runtime_error("Expected '<<' after cout, at line" + tokens[current].line);
        }

        Expr* expr = expression();

        if(!match({SEMICOLON})){
                    throw runtime_error("Expected ';' after expression, at line" + tokens[current-1].line);
        }

        return new PrintStmt(expr);
    }

    //input statements
    Stmt* InputStatement(){
        if(!match({INPUT})){
                    throw std::runtime_error("Expected '>>' after cin, at line" + tokens[current].line);
        }

        //makes sure a variable is there as that is where input is stored
        if(!match({VAR})){
                    throw std::runtime_error("Expected variable after >>, at line" + tokens[current].line);
        }

        Token name = previous();

        if(!match({SEMICOLON})){
                    throw std::runtime_error("Expected ';' after expression, at line" + tokens[current-1].line);
        }

        return new InputStmt(name);
    }

    //If statements
    Stmt* IfStatement(){
        if(!match({LEFT_PAREN})){
            throw runtime_error("Expected '(' after if, at line" + tokens[current].line);
        }
        Expr* condition = expression();
        if(!match({RIGHT_PAREN})){
            throw runtime_error("Expected ')' after condition, at line" + tokens[current].line);
        }

        Stmt* ifCode = bodyStatement();
        Stmt* elseCode = nullptr;

        if(match({ELSE})){
            if(match({IF})){
                elseCode = IfStatement();
            }
            else{
            elseCode = bodyStatement();
            }        
        }

        return new IfStmt(condition, ifCode, elseCode);
    }

    Stmt* bodyStatement(){
        if(match({LEFT_BRACE})){
                return blockStatement();
        }
        else{
            return statement();
        }
    }

    Stmt* blockStatement(){
        vector<Stmt*> statements;
        
        while(!check(RIGHT_BRACE)){
            if(isAtEnd()){
                throw runtime_error("Expected '}', at line" + tokens[current].line);
            }
            statements.push_back(declaration());
        }

        advance();

        return new BlockStmt(statements);
    }

    Stmt* expressionStatement(){
        Expr* expr = expression();

        if(!match({SEMICOLON})){
                    throw runtime_error("Expected ';' after expression, at line" + tokens[current].line);
        }
        
        return new ExprStmt(expr);
    }

    Stmt* whileStatement(){
        if(!match({LEFT_PAREN})){
            throw runtime_error("Expected '(' after if, at line" + tokens[current].line);
        }
        Expr* condition = expression();
        if(!match({RIGHT_PAREN})){
            throw runtime_error("Expected ')' after condition, at line" + tokens[current].line);
        }
        Stmt* body = bodyStatement();

        return new WhileStmt(condition, body);
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

            throw runtime_error("Invalid assignment target, at line" + tokens[current].line);
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
        if(match({NUMBER, STRING, TRUE, FALSE})){
            //cout << tokens[current-1].value;
            return new Value(previous());
        }
        else if(match({VAR})){
            return new Var(previous());
        }

        if(match({LEFT_PAREN})){
            Expr* expr = expression();

            if(!match({RIGHT_PAREN})){
                    throw runtime_error("Expected ';' after expression, at line" + tokens[current-1].line);
            }

            return new Group(expr);
        }
        throw runtime_error("Expected Value, at line" + tokens[current].line);
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
            return {};
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