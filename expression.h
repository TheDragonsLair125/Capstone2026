//AST Definitions
#ifndef EXPR_H_INCLUDED
#define EXPR_H_INCLUDED

#include "token.h"
#include <string>
#include "runtime.h"

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

#endif