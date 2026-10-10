#include "parser.hpp"
#include <iostream>

Parser::Parser(std::vector<Token> tokens) : tokens(tokens) {};

Token& Parser::getCurrent() {
    return tokens[current];
}

Token& Parser::advance() {
    return tokens[current++];
}

Token& Parser::peek() {
    return tokens[current + 1];
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
        auto expression = parseEquality();
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
            op.value,
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
            op.value,
            std::move(left),
            std::move(right)
        );
    }
    return left;
}

bool Parser::checkEqualityOperator() {
    return checkType(TokenType::IsEqual) || checkType(TokenType::NotEqual);
}
bool Parser::checkRelationalOperator() {
    return checkType(TokenType::LessEqual) || checkType(TokenType::GreaterEqual) 
    || checkType(TokenType::Greater) || checkType(TokenType::Less);
}

std::unique_ptr<Expr> Parser::parseEquality() {
    auto left = addition();
    while (checkEqualityOperator() || checkRelationalOperator()) {
        auto op = advance();
        auto right = addition();
        return std::make_unique<BinaryExpr>(
            op.value,
            std::move(left),
            std::move(right)
        );
    }
    return left;
}

std::unique_ptr<Statement> Parser::parseAssignment() {
    auto variable = consume(TokenType::Identifier);
    if (checkType(TokenType::PlusEqual) || checkType(TokenType::MinusEqual) || checkType(TokenType::Equal)) {
        auto opr = advance();
        auto right = parseEquality();
        auto expr = std::make_unique<AssignmentExpr>(opr.value, variable.value, std::move(right));
        return std::make_unique<AssignmentStatement>(std::move(expr));
    }

    else if (checkType(TokenType::Increment) || checkType(TokenType::Decrement)) {
        auto opr = advance();
        auto expr = std::make_unique<AssignmentExpr>(opr.value, variable.value, nullptr);
        return std::make_unique<AssignmentStatement>(std::move(expr));
    }

    else throw std::runtime_error("Unsupported assignment");
}

std::unique_ptr<Statement> Parser::parseEmit() {
    consume(TokenType::Emit);
    consume(TokenType::LeftParenthesis);
    auto expression = parseEquality();
    consume(TokenType::RightParenthesis);

    return std::make_unique<EmitStatement>(std::move(expression));
};

std::unique_ptr<Statement> Parser::parseIf() {
    consume(TokenType::IfStatement);
    consume(TokenType::LeftParenthesis);
    auto condition = parseEquality();
    consume(TokenType::RightParenthesis);
    consume(TokenType::LeftBrace);
    auto instructions = parseBlock();
    auto ifS = std::make_unique<IfStatement>(std::move(condition));
    ifS->instructions = std::move(instructions);
    if (checkType(TokenType::ElseStatement)) {
        if (peek().type == TokenType::IfStatement) {
            std::vector<std::unique_ptr<ElseIfBranch>> eifv;

            do {
                consume(TokenType::ElseStatement);
                if (!checkType(TokenType::IfStatement)) {
                    auto elseInstructions = parseElse();
                    ifS->elseInstructions = std::move(elseInstructions); 
                    break;
                }
                consume(TokenType::IfStatement);
                consume(TokenType::LeftParenthesis);
                auto condition = parseEquality();
                consume(TokenType::RightParenthesis);
                consume(TokenType::LeftBrace);
                auto instructions = parseBlock();
                auto u = std::make_unique<ElseIfBranch>(std::move(condition), std::move(instructions));
                eifv.push_back(std::move(u));
            } while (checkType(TokenType::ElseStatement));

            ifS->elseIfBranches = std::move(eifv);
        } 
        
        else {
            consume(TokenType::ElseStatement);
            auto elseInstructions = parseElse();
            ifS->elseInstructions = std::move(elseInstructions); 
        }
    }
    return ifS;
}

std::vector<std::unique_ptr<Statement>> Parser::parseElse() {
    consume(TokenType::LeftBrace);
    auto instructions = parseBlock();
    return instructions;
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

    auto value = parseEquality();

    return std::make_unique<VarDeclaration>(
        name,
        std::move(value),
        isConst
    );
}

std::vector<std::unique_ptr<Statement>> Parser::parseBlock() {
    std::vector<std::unique_ptr<Statement>> instructions;

    while (current < tokens.size()) {
        if (checkType(TokenType::ConstVar) || checkType(TokenType::Var)) {
            instructions.push_back(std::move(parseVariableDeclaration()));
        }

        else if (checkType(TokenType::Emit)) {
            instructions.push_back(std::move(parseEmit()));
        }

        else if (checkType(TokenType::IfStatement)) {
            instructions.push_back(std::move(parseIf()));
        }

        else if (checkType(TokenType::RightBrace)) {
            advance();
            break;
        }

        else {
            throw std::runtime_error("Expected Statement");
        }
    }
    return instructions;
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

        else if (checkType(TokenType::IfStatement)) {
            statements.push_back(std::move(parseIf()));
        }

        else if (checkType(TokenType::Identifier)) {
            statements.push_back(std::move(parseAssignment()));
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
