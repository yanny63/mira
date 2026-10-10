#pragma once

#include "token.hpp"
#include <memory>
#include <variant>
#include <vector>

using Value = std::variant<std::string, double, int, bool>;

struct Expr {
    virtual ~Expr() = default;
};

struct BinaryExpr : Expr {
    std::string op;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    BinaryExpr(std::string op, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) : op(op), left(std::move(left)), right(std::move(right)) {};
};

struct AssignmentExpr : Expr {
    std::string op;
    std::string variable;
    std::unique_ptr<Expr> right;
    AssignmentExpr(std::string op, std::string var, std::unique_ptr<Expr> right) : op(op), variable(var), right(std::move(right)) {};
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

    EmitStatement(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {};
};

struct ElseIfBranch {
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<Statement>> instructions;
};

struct IfStatement : Statement {
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<Statement>> instructions;
    std::vector<std::unique_ptr<ElseIfBranch>> elseIfBranches;
    std::vector<std::unique_ptr<Statement>> elseInstructions;
    IfStatement(std::unique_ptr<Expr> condition) : condition(std::move(condition)) {};
};

struct EvalStatement : Statement {
    std::unique_ptr<Expr> expression;
    EvalStatement(std::unique_ptr<Expr> expression) : expression(std::move(expression)) {};
};

struct AssignmentStatement : Statement {
    std::unique_ptr<Expr> expression;
    AssignmentStatement(std::unique_ptr<Expr> e) : expression(std::move(e)) {};
};

struct Variable {
    Value value;
    bool isConst;
};