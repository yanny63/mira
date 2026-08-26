#pragma once

#include <string>

enum class TokenType {
    ConstVar, 
    Var,

    Emit,
    Func,

    Variable,
    Identifier,
    Number,
    Double,
    String,

    Equal,
    Plus,
    Minus,
    Slash,
    Star,
    Percent,

    LeftParenthesis,
    RightParenthesis,

    Eof, 
};

struct Token {
    TokenType type;
    std::string value;
};