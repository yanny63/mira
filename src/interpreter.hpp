#pragma once
#include "ast.hpp"
#include <unordered_map>
#include <cmath>

class Interpreter {
    private:
    std::unordered_map<std::string, Variable> variables;
    Value evaluate(const Expr& e);

    public:
    void execute(const std::vector<std::unique_ptr<Statement>>& statements);
};