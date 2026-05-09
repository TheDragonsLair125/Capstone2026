#include <token.h>;

class Expr{
public:
    virtual ~Expr() = default;
};

class Math : public Expr{

    Expr* left;
    Token oper;
    Expr* right;

    Math(Expr* left, Token oper, Expr* right)
        : left(left), oper(oper), right(right) {}
};