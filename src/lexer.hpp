#pragma once

#include "token.hpp"
#include <string>
#include <vector>
#include <cctype>

class Lexer {
    private:
    std::string source;
    size_t position = 0;

    public:
    Lexer(std::string source);

    private:
    Token readNumber();
    Token readIdentifier();
    Token readString();

    public:
    std::vector<Token> tokenize();
};