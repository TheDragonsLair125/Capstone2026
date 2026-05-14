//AST Definitions
#ifndef EXPR_H_INCLUDED
#define EXPR_H_INCLUDED

#include <string>
#include <fstream>
#include "token.h"
#include "runtime.h"
#include "memorymap.h"

using namespace std;

class Math;
class Value;
class Var;
class Assign;

class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;

    virtual RuntimeVal visitMath(Math* expr) = 0;
    virtual RuntimeVal visitValue(Value* expr) = 0;
    virtual RuntimeVal visitVar(Var* expr) = 0;
    virtual RuntimeVal visitAssign(Assign* expr) = 0;
};

class Expr{
public:
    virtual ~Expr() = default;

    virtual RuntimeVal accept(ExprVisitor* visitor) = 0;
};

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

class Value : public Expr{
public:
    Token value;
    
    Value(Token value)
    :value(value){}

    RuntimeVal accept(ExprVisitor* visitor) override {
        return visitor->visitValue(this);
    }

};

class Var : public Expr{
public:
    Token var;
    
    Var(Token var)
    :var(var){}

    RuntimeVal accept(ExprVisitor* visitor) override {
        return visitor->visitVar(this);
    }

};

class Assign : public Expr{
public:
    Token name;
    Expr* value;

    Assign(Token name, Expr* value)
    : name(name), value(value){}

    RuntimeVal accept(ExprVisitor* visitor){
        return visitor->visitAssign(this);
    }
};

class ExprStmt;
class PrintStmt;
class VarStmt;
class InputStmt;

class StmtVisitor{
public:
    virtual ~StmtVisitor() = default;

    virtual void visitExprStmt(ExprStmt* stmt) = 0;
    virtual void visitPrintStmt(PrintStmt* stmt) = 0;
    virtual void visitVarStmt(VarStmt* stmt) = 0;
    virtual void visitInputStmt(InputStmt* stmt) = 0;
};

class Stmt{
public:
    virtual ~Stmt() = default;

    virtual void accept(StmtVisitor* visitor) = 0;
};

class ExprStmt : public Stmt{
public:
    Expr* expr;

    ExprStmt(Expr* expr)
    :expr(expr){}

    void accept(StmtVisitor* visitor) override {
        return visitor->visitExprStmt(this);
    }
};

class PrintStmt : public Stmt{
public:
    Expr* expr;

    PrintStmt( Expr* expr)
    : expr(expr){}

    void accept(StmtVisitor* visitor) override {
        return visitor->visitPrintStmt(this);
    }
};

class InputStmt: public Stmt{
public:
    Token name;

    InputStmt(Token name)
    : name(name){}

    void accept(StmtVisitor* visitor) override {
        return visitor->visitInputStmt(this);
    }
};

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

class Interpeter : public ExprVisitor, public StmtVisitor{
private:
    RuntimeVal evaluate(Expr* expr){
        return expr->accept(this);
    }

    void execute(Stmt* stmt){
        return stmt->accept(this);
    }

    std::ofstream outputFile;
    MemoryMap memory;

public:
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

        return tempVal;
    }

    RuntimeVal visitMath(Math* expr) override{
        RuntimeVal left = evaluate(expr->left);
        RuntimeVal right = evaluate(expr->right);

        RuntimeVal result;
        result.type = NUMBER_VAL;

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

            default:
                return RuntimeVal(); // nil
                break;
        }

    }
    
    RuntimeVal visitVar(Var* expr) override{
        return memory.get(expr->var);
    }

    RuntimeVal visitAssign(Assign* expr) override{
        RuntimeVal value = evaluate(expr->value);

        memory.assign(expr->name, value);

        return value;
    }

    void visitExprStmt(ExprStmt* stmt) override{
        RuntimeVal value = evaluate(stmt->expr);
        cout << toString(value) << endl;

        return;
    }

    void visitPrintStmt(PrintStmt* stmt) override{
        RuntimeVal value = evaluate(stmt->expr);
        cout << toString(value) << endl;
        outputFile << toString(value) << endl;

        return;
    }

    void visitInputStmt(InputStmt* stmt) override{
        Variable variable = memory.getVariable(stmt->name);

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

    void visitVarStmt(VarStmt* stmt) override{
        Variable variable;
        variable.declaredType = stmt->declaredType;

        if(stmt->expr != nullptr){
            variable.value = evaluate(stmt->expr);

            if(variable.value.type != variable.declaredType){
                throw runtime_error("Type mismatch in variable declaration for: " + stmt->name.value + " ,at line: " + std::to_string(stmt->name.line));
            }
        }

        else{
            variable.value = RuntimeVal();
        }

        memory.define(stmt->name.value, variable);
    }

    void interpret(Stmt* stmt){
        try{
            execute(stmt);
        }
        catch(const exception& e){
            cout << e.what() << endl;
        }
    }

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
    
    Interpeter(){
        outputFile.open("output.txt");
    }

    ~Interpeter(){
        outputFile.close();
    }

};

#endif