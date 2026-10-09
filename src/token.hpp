#pragma once

#include <string>

enum class TokenType {
    ConstVar, 
    Var,

    Emit,
    Fn,
    IfStatement,
    ElseStatement,

    Variable,
    Identifier,
    Number,
    Double,
    String,

    Equal,
    IsEqual,
    NotEqual,
    LessEqual,
    Less,
    Greater,
    GreaterEqual,
    Plus,
    Minus,
    Slash,
    Star,
    Percent,

    LeftParenthesis,
    RightParenthesis,
    LeftBrace,
    RightBrace,

    Eof, 
};

struct Token {
    TokenType type;
    std::string value;
};