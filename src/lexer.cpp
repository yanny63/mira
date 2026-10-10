#include "lexer.hpp"
#include <iostream>

Lexer::Lexer(std::string source) : source(std::move(source)) {};

Token Lexer::readNumber() {
    std::string number;

    while (position < source.size() && isdigit(source[position]) || source[position] == '.') {
        number += source[position];
        position++;
    }

    if (number.find(".") == std::string::npos) {
        return {
            TokenType::Number,
            number
        };
    }
    else if (number.find(".") != std::string::npos) {
        return {
            TokenType::Double,
            number
        };
    }
    else {
        throw std::runtime_error("Unexpected number");
    }
};

Token Lexer::readIdentifier() {
    std::string identifier;

    while (position < source.size() && isalpha(source[position]) || source[position] == '_') {
        identifier += source[position];
        position++;
    };

    if (identifier == "const") {
        return {
            TokenType::ConstVar,
            identifier
        };
    }

    if (identifier == "var") {
        return {
            TokenType::Var,
            identifier
        };
    }

    if (identifier == "emit") {
        return {
            TokenType::Emit,
            identifier
        };
    }

    if (identifier == "if") {
        return {
            TokenType::IfStatement,
            identifier
        };
    }

    if (identifier == "else") {
        return {
            TokenType::ElseStatement,
            identifier
        };
    }

    if (identifier == "while") {
        return {
            TokenType::WhileStatement,
            identifier
        };
    }
    // default return (variables etc)
    return {
        TokenType::Identifier,
        identifier
    };
};

Token Lexer::readString() {
    std::string string;
    
    position++;
    while (position < source.size() && source[position] != '"') {
        string += source[position];
        position++;
    }

    if (position >= source.size()) {
        throw std::runtime_error("Unterminated string");
    }

    position++;

    return {
        TokenType::String,
        string
    };
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (position < source.size()) {
        char current = source[position];

        if (isspace(current)) {
            position++;
            continue;
        }

        if (isalpha(current) || source[position] == '_') {
            tokens.push_back(readIdentifier());
            continue;
        }

        if (isdigit(current)) {
            tokens.push_back(readNumber());
            continue;
        }

        if (current == '"') {
            tokens.push_back(readString());
            continue;
        }

        switch (current) 
        {   
            case '=':
                position++;
                if (source[position] == '=') {
                    tokens.push_back({TokenType::IsEqual, "=="});
                    position++;
                    break;
                }
                tokens.push_back({TokenType::Equal, "="});
                break;
            case '+':
                position++;
                if (source[position] == '=') {
                    tokens.push_back({TokenType::PlusEqual, "+="});
                    position++;
                    break;
                }
                if (source[position] == '+') {
                    tokens.push_back({TokenType::Increment, "++"});
                    position++;
                    break;
                }
                tokens.push_back({TokenType::Plus, "+"});
                break;
            case '-':
                position++;
                if (source[position] == '=') {
                    tokens.push_back({TokenType::MinusEqual, "-="});
                    position++;
                    break;
                }
                if (source[position] == '-') {
                    tokens.push_back({TokenType::Decrement, "--"});
                    position++;
                    break;
                }
                tokens.push_back({TokenType::Minus, "-"});
                break;
            case '*':
                position++;
                tokens.push_back({TokenType::Star, "*"});
                break;
            case '/':
                position++;
                tokens.push_back({TokenType::Slash, "/"});
                break;
            case '%': 
                position++;
                tokens.push_back({TokenType::Percent, "%"});
                break;
            case '!':
                position++;
                if (source[position] == '=') {
                    tokens.push_back({TokenType::NotEqual, "!="});
                    position++;
                    break;
                }
                break;
            case '<':
                position++;
                if (source[position] == '=') {
                    tokens.push_back({TokenType::LessEqual, "<="});
                    position++;
                    break;
                }
                tokens.push_back({TokenType::Less, "<"});
                break;
            case '>':
                position++;
                if (source[position] == '=') {
                    tokens.push_back({TokenType::GreaterEqual, ">="});
                    position++;
                    break;
                }
                tokens.push_back({TokenType::Greater, ">"});
                break;
            case '(':
                position++;
                tokens.push_back({TokenType::LeftParenthesis, "("});
                break;
            case ')':
                position++;
                tokens.push_back({TokenType::RightParenthesis, ")"});
                break;
            case '{':
                position++;
                tokens.push_back({TokenType::LeftBrace, "{"});
                break;
            case '}':
                position++;
                tokens.push_back({TokenType::RightBrace, "}"});
                break;
            default:
                std::cerr << "Unknown character: " << current << '\n';
                position++;
                break;
        }
    }

    tokens.push_back({
        TokenType::Eof,
        ""
    });

    return tokens;
};