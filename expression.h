//AST Definitions
#ifndef EXPR_H_INCLUDED
#define EXPR_H_INCLUDED

#include "token.h"
#include <string>
#include "runtime.h"

using namespace std;

class Math;
class Value;

class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;

    virtual RuntimeVal visitMath(Math* expr) = 0;
    virtual RuntimeVal visitValue(Value* expr) = 0;
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

class ExprStmt;

class StmtVisitor{
public:
    virtual ~StmtVisitor() = default;

    virtual void visitExprStmt(ExprStmt* stmt) = 0;
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

class Interpeter : public ExprVisitor, public StmtVisitor{
public:
    RuntimeVal visitValue(Value* expr) override{
        RuntimeVal tempVal;
        tempVal.type = NUMBER_VAL;
        tempVal.numberVal = stod(expr->value.value);
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

            case DIVIDE:
                result.numberVal = left.numberVal / right.numberVal;
                return result;

            case MULTIPLY:
                result.numberVal = left.numberVal * right.numberVal;
                return result;

            case PLUS:
                result.numberVal = left.numberVal + right.numberVal;
                return result;

            default:
                return RuntimeVal(); // nil
        }

    }

    void visitExprStmt(ExprStmt* stmt) override{
        RuntimeVal value = evaluate(stmt->expr);
        cout << toString(value) << endl;

        return;
    }

    void interpret(Stmt* stmt){
        try{
            execute(stmt);
        }
        catch(const exception& e){
            cout << "error";
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

    private:
    RuntimeVal evaluate(Expr* expr){
        return expr->accept(this);
    }

    void execute(Stmt* stmt){
        return stmt->accept(this);
    }

};

#endif