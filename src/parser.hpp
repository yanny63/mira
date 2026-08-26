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

    bool checkType(TokenType type);

    Token consume(TokenType type);
    
    std::unique_ptr<Expr> primary();

    std::unique_ptr<Expr> multiplication();
    std::unique_ptr<Expr> addition();

    std::unique_ptr<Statement> parseEmit();

    std::unique_ptr<Expr> parseString();

    std::unique_ptr<Expr> parseExpresion();

    std::unique_ptr<Statement> parseVariableDeclaration();

    public:
    std::vector<std::unique_ptr<Statement>> parse();
};