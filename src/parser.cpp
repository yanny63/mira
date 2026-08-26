#include "parser.hpp"
#include <iostream>

Parser::Parser(std::vector<Token> tokens) : tokens(tokens) {};

Token& Parser::getCurrent() {
    return tokens[current];
}

Token& Parser::advance() {
    return tokens[current++];
}

bool Parser::checkType(TokenType type) {
    return tokens[current].type == type;
}

Token Parser::consume(TokenType type) {
    if (getCurrent().type != type) {
        throw std::runtime_error("Unexpected token");
    }

    return advance();
}

std::unique_ptr<Expr> Parser::primary()
{
    if (checkType(TokenType::Number))
    {   
        return std::make_unique<NumberExpr>(
            std::stoi(consume(TokenType::Number).value)
        );
    }

    if (checkType(TokenType::Double)) {
        return std::make_unique<DoubleExpr>(
            std::stod(consume(TokenType::Double).value)
        );
    }

    if (checkType(TokenType::String))
    {
        return std::make_unique<StringExpr>(
            consume(TokenType::String).value
        );
    }

    if (checkType(TokenType::LeftParenthesis))
    {
        advance();
        auto expression = parseExpresion();
        consume(TokenType::RightParenthesis);
        return expression;
    }

    if (checkType(TokenType::Identifier)) {
        return std::make_unique<VarExpr>(consume(TokenType::Identifier).value);
    }

    throw std::runtime_error("Expected expression");
}

std::unique_ptr<Expr> Parser::multiplication() {
    auto left = primary();

    while (checkType(TokenType::Star) || checkType(TokenType::Slash) || checkType(TokenType::Percent)) {
        Token op = advance();
        auto right = primary();

        left = std::make_unique<BinaryExpr>(
            op.value[0],
            std::move(left),
            std::move(right)
        );
    }

    return left;
}

std::unique_ptr<Expr> Parser::addition() {
    auto left = multiplication();

    while (checkType(TokenType::Plus) || checkType(TokenType::Minus)) {
        Token op = advance();
        auto right = multiplication();

        left = std::make_unique<BinaryExpr>(
            op.value[0],
            std::move(left),
            std::move(right)
        );
    }
    return left;
}

std::unique_ptr<Statement> Parser::parseEmit() {
    consume(TokenType::Emit);
    consume(TokenType::LeftParenthesis);
    auto expression = parseExpresion();
    consume(TokenType::RightParenthesis);

    return std::make_unique<EmitStatement>(std::move(expression));
};

std::unique_ptr<Expr> Parser::parseExpresion() {
    return addition();
}

std::unique_ptr<Statement> Parser::parseVariableDeclaration()
{
    bool isConst = false;

    if (checkType(TokenType::ConstVar))
    {
        isConst = true;
        advance();
    }

    consume(TokenType::Var);

    std::string name = consume(TokenType::Identifier).value;

    consume(TokenType::Equal);

    auto value = parseExpresion();

    return std::make_unique<VarDeclaration>(
        name,
        std::move(value),
        isConst
    );
}

std::vector<std::unique_ptr<Statement>> Parser::parse() {
    std::vector<std::unique_ptr<Statement>> statements;

    while (current < tokens.size()) {
        if (checkType(TokenType::ConstVar) || checkType(TokenType::Var)) {
            statements.push_back(std::move(parseVariableDeclaration()));
        }

        else if (checkType(TokenType::Emit)) {
            statements.push_back(std::move(parseEmit()));
        }
        else if (checkType(TokenType::Eof)) {
            break;
        }
        else {
            throw std::runtime_error("Expected Statement");
        }
    }
    return statements;
}
