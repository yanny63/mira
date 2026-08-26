#pragma once

#include "token.hpp"
#include <memory>
#include <variant>

using Value = std::variant<std::string, double, int, bool>;

struct Expr {
    virtual ~Expr() = default;
};

struct BinaryExpr : Expr {
    char op;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    BinaryExpr(char op, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) : op(op), left(std::move(left)), right(std::move(right)) {};
};

struct StringExpr : Expr {
    std::string string;
    StringExpr(std::string string) : string(string) {};
};

struct NumberExpr : Expr {
    int number;
    NumberExpr(int number) : number(number) {};
};

struct DoubleExpr : Expr {
    double number;
    DoubleExpr(double number) : number(number) {};
};

struct VarExpr : Expr {
    std::string var;
    VarExpr(std::string var) : var(var) {};
};

struct Statement {
    virtual ~Statement() = default;
};

struct VarDeclaration : Statement {
    std::string identifier;
    std::unique_ptr<Expr> value;
    bool isConst;

    VarDeclaration(std::string identifier, std::unique_ptr<Expr> value, bool isConst) : identifier(identifier), value(std::move(value)), isConst(isConst) {};
};

struct EmitStatement : Statement {
    std::unique_ptr<Expr> expression;

    EmitStatement(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {}
};

struct Variable {
    Value value;
    bool isConst;
};