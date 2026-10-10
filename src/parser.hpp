#pragma once

#include "token.hpp"
#include "ast.hpp"
#include <vector>

class Parser {
    private:
    std::vector<Token> tokens;
    size_t current = 0;

    public:
    Parser(std::vector<Token> tokens);

    private:
    Token& getCurrent();

    Token& advance();

    Token& peek();

    bool checkType(TokenType type);

    Token consume(TokenType type);
    
    std::unique_ptr<Expr> primary();

    std::unique_ptr<Expr> multiplication();
    std::unique_ptr<Expr> addition();

    std::unique_ptr<Statement> parseEmit();

    std::unique_ptr<Statement> parseIf();
    std::vector<std::unique_ptr<Statement>> parseElse();

    std::unique_ptr<Expr> parseString();

    std::unique_ptr<Expr> parseExpression();

    bool checkEqualityOperator();
    bool checkRelationalOperator();

    std::unique_ptr<Expr> parseEquality();

    std::unique_ptr<Statement> parseAssignment();

    std::unique_ptr<Statement> parseVariableDeclaration();

    std::vector<std::unique_ptr<Statement>> parseBlock();

    public:
    std::vector<std::unique_ptr<Statement>> parse();
};