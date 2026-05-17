//AST Definitions along with the interpeter for the project
#ifndef EXPR_H_INCLUDED
#define EXPR_H_INCLUDED
//includes
#include <string>
#include <fstream>
#include "token.h"
#include "runtime.h"
#include "memorymap.h"

using namespace std;

//classes declared up here for visitor function
class Math;
class Value;
class Var;
class Assign;
class Group;
class Unary;

///////////////////////////////////////////////////////////////////////////////
// Expression AST/Class definitions
///////////////////////////////////////////////////////////////////////////////
//visitor functiona as this program implements Visitor pattern to allow for easier scaling of the project overtime
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;

    //set up for visitor functions
    virtual RuntimeVal visitMath(Math* expr) = 0;
    virtual RuntimeVal visitValue(Value* expr) = 0;
    virtual RuntimeVal visitVar(Var* expr) = 0;
    virtual RuntimeVal visitAssign(Assign* expr) = 0;
    virtual RuntimeVal visitGroup(Group* expr) = 0;
    virtual RuntimeVal visitUnary(Unary* expr) = 0;
};

//base class for expressions
class Expr{
public:
    virtual ~Expr() = default;

    virtual RuntimeVal accept(ExprVisitor* visitor) = 0;
};

//Ast for math functions expression opertaion expression | value
class Math : public Expr{
public:
    Expr* left;
    Token oper;
    Expr* right;

    Math(Expr* left, Token oper, Expr* right)
        : left(left), oper(oper), right(right) {}

    RuntimeVal accept(ExprVisitor* visitor) override {
        return visitor->visitMath(this);
    }

};

//AST for value functions Number | String | "True" | "False"
class Value : public Expr{
public:
    Token value;
    
    Value(Token value)
    :value(value){}

    RuntimeVal accept(ExprVisitor* visitor) override {
        return visitor->visitValue(this);
    }

};

//AST for variables VARIABLE  variable
class Var : public Expr{
public:
    Token var;
    
    Var(Token var)
    :var(var){}

    RuntimeVal accept(ExprVisitor* visitor) override {
        return visitor->visitVar(this);
    }

};

//AST for variable assignment variable = expression
class Assign : public Expr{
public:
    Token name;
    Expr* value;

    Assign(Token name, Expr* value)
    : name(name), value(value){}

    RuntimeVal accept(ExprVisitor* visitor) override{
        return visitor->visitAssign(this);
    }
};

//AST for parantheses "("  expression  ")"
class Group : public Expr{
public:
    Expr* expr;

    Group(Expr* expr)
    : expr(expr){}

    RuntimeVal accept(ExprVisitor* visitor) override{
        return visitor->visitGroup(this);
    }

};

//Unary expression AST "-" expression | "!" expression
class Unary : public Expr{
public:
    Token oper;
    Expr* right;

    Unary(Token oper, Expr* right)
    :oper(oper), right(right){}

    RuntimeVal accept(ExprVisitor* visitor) override{
        return visitor->visitUnary(this);
    }
};

///////////////////////////////////////////////////////////////////////////////
// Statements AST/Class definitions
///////////////////////////////////////////////////////////////////////////////

class ExprStmt;
class PrintStmt;
class VarStmt;
class InputStmt;
class IfStmt;
class BlockStmt;
class WhileStmt;

//visitor function for statments
class StmtVisitor{
public:
    virtual ~StmtVisitor() = default;

    virtual void visitExprStmt(ExprStmt* stmt) = 0;
    virtual void visitPrintStmt(PrintStmt* stmt) = 0;
    virtual void visitVarStmt(VarStmt* stmt) = 0;
    virtual void visitInputStmt(InputStmt* stmt) = 0;
    virtual void visitIfStmt(IfStmt* stmt) = 0;
    virtual void visitBlockStmt(BlockStmt* stmt) = 0;
    virtual void visitWhileStmt(WhileStmt* stmt) = 0;
};

//statement base class
class Stmt{
public:
    virtual ~Stmt() = default;

    virtual void accept(StmtVisitor* visitor) = 0;
};

//expression statement  ast expression program | expression
class ExprStmt : public Stmt{
public:
    Expr* expr;

    ExprStmt(Expr* expr)
    :expr(expr){}

    void accept(StmtVisitor* visitor) override {
        return visitor->visitExprStmt(this);
    }
};

//print statement ast cout << expression
class PrintStmt : public Stmt{
public:
    Expr* expr;

    PrintStmt( Expr* expr)
    : expr(expr){}

    void accept(StmtVisitor* visitor) override {
        return visitor->visitPrintStmt(this);
    }
};

//input statement ast cin >> variable
class InputStmt: public Stmt{
public:
    Token name;

    InputStmt(Token name)
    : name(name){}

    void accept(StmtVisitor* visitor) override {
        return visitor->visitInputStmt(this);
    }
};

//input statement ast variable decleration type variable = expression | type variable
class VarStmt : public Stmt{
public:
    Token name;
    ValueType declaredType;
    Expr* expr;

    VarStmt(Token name, ValueType declaredType, Expr* expr)
    : name(name), declaredType(declaredType), expr(expr){}

    void accept(StmtVisitor* visitor) override {
        return visitor->visitVarStmt(this);
    }

};

//if statement ast if(expression) block if | if(expression) block
class IfStmt : public Stmt{
public:
    Expr* condition;
    Stmt* ifCode;
    Stmt* elseCode;

    IfStmt(Expr* condition, Stmt* ifCode, Stmt* elseCode)
    : condition(condition), ifCode(ifCode), elseCode(elseCode){}

    void accept(StmtVisitor* visitor) override{
        return visitor->visitIfStmt(this);
    }
};

//block statement ast "{" statement "}"
class BlockStmt : public Stmt{
public:
    vector<Stmt*> statements;

    BlockStmt(vector<Stmt*> statements)
    :statements(statements){}

    void accept(StmtVisitor* visitor){
        return visitor->visitBlockStmt(this);
    }
};

//while statemetn ast "while (" expression ")" block
class WhileStmt : public Stmt{
public:
    Expr* condition;
    Stmt* body;

    WhileStmt(Expr* condition, Stmt* body)
    :condition(condition), body(body){}

    void accept(StmtVisitor* visitor){
        return visitor->visitWhileStmt(this);
    }
};

///////////////////////////////////////////////////////////////////////////////
// Interpeter: Interpets AST into actual runeable code
///////////////////////////////////////////////////////////////////////////////
class Interpeter : public ExprVisitor, public StmtVisitor{
private: 
    //helper functions
    RuntimeVal evaluate(Expr* expr){
        return expr->accept(this);
    }

    void execute(Stmt* stmt){
        return stmt->accept(this);
    }

    //output enviroment decleration
    std::ofstream outputFile;
    MemoryMap memory;

public:
///////////////////////////////////////////////////////////////////////////////
// Interpeter code for expressions
///////////////////////////////////////////////////////////////////////////////
    //visit function for value, creates RuntimeVal object of appropiate type based on token indentifier
    RuntimeVal visitValue(Value* expr) override{
        RuntimeVal tempVal;

        if(expr->value.type == NUMBER){
            tempVal.type = NUMBER_VAL;
            tempVal.numberVal = stod(expr->value.value);
        }
        
        else if(expr->value.type == STRING){
            tempVal.type = STRING_VAL;
            tempVal.stringVal = expr->value.value;
        }

        //both set for bool val  cause true and false
        else if(expr->value.type == TRUE){
            tempVal.type = BOOL_VAL;
            tempVal.boolVal = true;
        }

        else if(expr->value.type == FALSE){
            tempVal.type = BOOL_VAL;
            tempVal.boolVal = false;
        }

        return tempVal;
    }

    //visit function for Math
    RuntimeVal visitMath(Math* expr) override{
        //gets left expresion and  right expression to evaluate before math
        RuntimeVal left = evaluate(expr->left);
        RuntimeVal right = evaluate(expr->right);

        RuntimeVal result;
        result.type = NUMBER_VAL;

        //switch for each type of operation
        switch (expr->oper.type) {
            case MINUS:
                result.numberVal = left.numberVal - right.numberVal;
                return result;
                break;

            case DIVIDE:
                result.numberVal = left.numberVal / right.numberVal;
                return result;
                break;

            case MULTIPLY:
                result.numberVal = left.numberVal * right.numberVal;
                return result;
                break;

            case PLUS:
                if(left.type == NUMBER_VAL && right.type == NUMBER_VAL){
                    result.numberVal = left.numberVal + right.numberVal;
                    return result;
                }
                
                if(left.type == STRING_VAL || right.type == STRING_VAL){
                    if(left.type == NUMBER_VAL){
                        left.stringVal = toString(left);
                    }
                    if(right.type == NUMBER_VAL){
                        right.stringVal = toString(right);
                    }             
                    
                    result.type = STRING_VAL;
                    result.stringVal = left.stringVal + right.stringVal;
                    return result;
                }

                break;

            case LESS_THAN:
                result.type = BOOL_VAL;
                result.boolVal = left.numberVal < right.numberVal;
                return result;
                break;

            case LESS_THAN_EQUAL:
                result.type = BOOL_VAL;
                result.boolVal = left.numberVal <= right.numberVal;
                return result;
                break;

            case GREATER_THAN:
                result.type = BOOL_VAL;
                result.boolVal = left.numberVal > right.numberVal;
                return result;
                break;

            case GREATER_THAN_EQUAL:
                result.type = BOOL_VAL;
                result.boolVal = left.numberVal >= right.numberVal;
                return result;
                break;

            case EQUAL_TO:
            //checks to make sure type of comparison is same to prevent comparison mismatch
                result.type = BOOL_VAL;
                if(left.type == NUMBER_VAL && right.type == NUMBER_VAL){
                    result.boolVal = left.numberVal == right.numberVal;
                }

                else if(left.type == STRING_VAL && right.type == STRING_VAL){
                    result.boolVal = left.stringVal == right.stringVal;
                }

                else if(left.type == BOOL_VAL && right.type == BOOL_VAL){
                    result.boolVal = left.boolVal == right.boolVal;
                }
                
                else{
                    throw std::runtime_error("Can not compare type " + to_string(left.type) + " to " + to_string(right.type) + " at line: " + std::to_string(expr->oper.line));
                }
                return result;
                break;
            
            case NOT_EQUAL_TO:
            //checks to make sure type of comparison is same to prevent comparison mismatch
                result.type = BOOL_VAL;
                if(left.type == NUMBER_VAL && right.type == NUMBER_VAL){
                    result.boolVal = left.numberVal != right.numberVal;
                }

                else if(left.type == STRING_VAL && right.type == STRING_VAL){
                    result.boolVal = left.stringVal != right.stringVal;
                }

                else if(left.type == BOOL_VAL && right.type == BOOL_VAL){
                    result.boolVal = left.boolVal != right.boolVal;
                }
                
                else{
                    throw std::runtime_error("Can not compare type " + to_string(left.type) + " to " + to_string(right.type) + " at line: " + std::to_string(expr->oper.line));
                }
                return result;
                break;

            default:
            //default to return for NULL / just in case
                return RuntimeVal(); // null
                break;
        }

    }
    
    //gets variable from memory map
    RuntimeVal visitVar(Var* expr) override{
        return memory.get(expr->var);
    }

    //assigns variables by using the assign function
    RuntimeVal visitAssign(Assign* expr) override{
        RuntimeVal value = evaluate(expr->value);

        memory.assign(expr->name, value);

        return value;
    }

    //visit for parenthesis
    RuntimeVal visitGroup(Group* expr) override{
        return evaluate(expr->expr);
    }

    //visit function for unary 
    RuntimeVal visitUnary(Unary* expr) override{
        RuntimeVal right = evaluate(expr->right);

        switch(expr->oper.type){
            //if minus takes value and makes it negative if valid target
            case MINUS:{
                if(right.type != NUMBER_VAL){
                    throw runtime_error("Operand must be number at line: " + std::to_string(expr->oper.line));
                }

                RuntimeVal result;
                result.type = NUMBER_VAL;
                result.numberVal = -right.numberVal;
                return result;
                break;
            }

            //gets reverse true / false from current expression
            case NOT:{
                RuntimeVal result;
                result.type = BOOL_VAL;
                result.boolVal = !isTrue(right);

                return result;
            }
            
        }
        
        return RuntimeVal();
    }

    //checks if value is true or false helper for above
        bool isTrue(RuntimeVal value){

            //like c++ null defaults to false
        if(value.type == NULL_VAL){
            return false;
        }

        if(value.type == BOOL_VAL){
            return value.boolVal;
        }

        return true;
    }
///////////////////////////////////////////////////////////////////////////////
// Interpeter for statements
///////////////////////////////////////////////////////////////////////////////

    //visitor for expression statement
    void visitExprStmt(ExprStmt* stmt) override{
        RuntimeVal value = evaluate(stmt->expr);
        //goes to custom to string function for demo / debug reasons
        cout << toString(value) << endl;

        return;
    }

    //visitor for print statements
    void visitPrintStmt(PrintStmt* stmt) override{
        RuntimeVal value = evaluate(stmt->expr);
        cout << toString(value) << endl;
        //outputs to text file to seperate output to normal text
        outputFile << toString(value) << endl;

        return;
    }

    //visitor for inputs
    void visitInputStmt(InputStmt* stmt) override{
        //gets proper variable information
        Variable variable = memory.getVariable(stmt->name);

        //sees what type of variable is gotten to get input for proper memory management
        if(variable.declaredType == NUMBER_VAL){

            double input;
            std::cin >> input;

            RuntimeVal value;
            value.type = NUMBER_VAL;
            value.numberVal = input;

            memory.assign(stmt->name, value);
        }

        else if(variable.declaredType == STRING_VAL){

            std::string input;
            std::cin >> input;

            RuntimeVal value;
            value.type = STRING_VAL;
            value.stringVal = input;

            memory.assign(stmt->name, value);
        }
    }

    //variable creation visitor
    void visitVarStmt(VarStmt* stmt) override{
        Variable variable;
        variable.declaredType = stmt->declaredType;

        //checks if variable should have value or be null on creation
        if(stmt->expr != nullptr){
            variable.value = evaluate(stmt->expr);

            if(variable.value.type != variable.declaredType){
                throw runtime_error("Type mismatch in variable declaration for: " + stmt->name.value + " ,at line: " + std::to_string(stmt->name.line));
            }
        }

        else{
            variable.value = RuntimeVal();
        }

        //once variable creation value decided creates in memory
        memory.define(stmt->name.value, variable);
    }

    //visitor for if statements
    void visitIfStmt(IfStmt* stmt) override{
        //makes sure chosen condition is true before executing
        if(isTrue(evaluate(stmt->condition))){
            execute(stmt->ifCode);
        }
        
        //sees if there is an else or else if if not doesnt run so can have only ifs
        else if(stmt->elseCode != nullptr){
            execute(stmt->elseCode);
        }

        return;
    }

    //visit statement for blocks was two functions in case could get local memory set up
    void visitBlockStmt(BlockStmt* stmt) override{
        executeBlock(stmt->statements);
    }

    void executeBlock(vector<Stmt*> statements){
        //does for to make sure every statement in block runs
        for(Stmt* statement: statements){
            execute(statement);
        }
    }

    void visitWhileStmt(WhileStmt* stmt) override{
        //runs code inside while conditon is true
        while (isTrue(evaluate(stmt->condition))){
            execute(stmt->body);
        }
    }

///////////////////////////////////////////////////////////////////////////////
// Interpeter meta functions
///////////////////////////////////////////////////////////////////////////////
//trys to interpet otherwise exports the error
    void interpret(Stmt* stmt){
        try{
            execute(stmt);
        }
        catch(const exception& e){
            cout << e.what() << endl;
        }
    }

    //turns current data into a string for output based on its value type
    string toString(RuntimeVal value){
        if (value.type == NULL_VAL){
            return "null";
        }

        if(value.type == NUMBER_VAL){
            
            string text = to_string(value.numberVal);

            if(text.find('.') != string::npos){

                while(!text.empty() && text.back() == '0'){
                    text.pop_back();
                }

                if(!text.empty() && text.back() == '.'){
                    text.pop_back();
                }
            }

            return text;
        }

        if (value.type == STRING_VAL){
            return value.stringVal;
        }

        if (value.type == BOOL_VAL){

            if(value.boolVal){
                return "true";
            }

            else{
                return "false";
            }
        }

        return "uhh oh something went wrong interpeter side\n"; 
    }
    
    //creates file on startup
    Interpeter(){
        outputFile.open("output.txt");
    }

    //closes file on program end
    ~Interpeter(){
        outputFile.close();
    }

};

#endif