#include "interpreter.hpp"
#include <iostream>
#include <type_traits>

Value Interpreter::evaluate(const Expr& e) {
    if (auto expr = dynamic_cast<const NumberExpr*>(&e)) {
        return expr->number;
    }

    if (auto expr = dynamic_cast<const StringExpr*>(&e)) {
        return expr->string;
    }

    if (auto expr = dynamic_cast<const VarExpr*>(&e)) {
        return variables.at(expr->var).value;
    }

    if (auto expr = dynamic_cast<const BinaryExpr*>(&e)) {
        Value left = evaluate(*expr->left);
        Value right = evaluate(*expr->right);

        return std::visit([&](const auto& l, const auto& r) -> Value {
            using L = std::decay_t<decltype(l)>;
            using R = std::decay_t<decltype(r)>;

            if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
                switch (expr->op) {
                    case '+':
                        return l + r;
                    case '-':
                        return l - r;
                    case '*':
                        return l * r;
                    case '/':
                        if (r == 0) {
                            throw std::runtime_error("Can't divide by 0");
                        }
                        return l / r;
                    case '%':
                        if (r == 0) {
                            throw std::runtime_error("Can't divide by 0");
                        }
                        return std::fmod(l, r);
                    default:
                        throw std::runtime_error("Unknown operator");
                }
            }

            else if constexpr (std::is_same_v<L, std::string> && std::is_arithmetic_v<R>) {
                if (expr->op == '+') {
                    return l + std::to_string(r);
                }
                throw std::runtime_error("Invalid operation between string and number");
            }

            else if constexpr (std::is_same_v<R, std::string> && std::is_arithmetic_v<L>) {
                if (expr->op == '+') {
                    return std::to_string(l) + r;
                }
                throw std::runtime_error("Invalid operation between string and number");
            }

            else if constexpr (std::is_same_v<L, std::string> && std::is_same_v<R, std::string>) {
                if (expr->op == '+') {
                    return l + r;
                } 
                throw std::runtime_error("Invalid operator for strings");
            }

            else {
                throw std::runtime_error("Invalid operands for binary operation");
            }
        }, left, right);
    }
          
    throw std::runtime_error("Unknown expression type");
}

void Interpreter::execute(const std::vector<std::unique_ptr<Statement>>& statements) {
    for (const auto& e : statements) {
        if (auto expr = dynamic_cast<const VarDeclaration*>(e.get())) {
            Value value = evaluate(*expr->value);

            variables[expr->identifier] = {value, expr->isConst};
        }

        else if (auto expr = dynamic_cast<const EmitStatement*>(e.get())) {
            Value toEmit = evaluate(*expr->expression);

            if (std::holds_alternative<double>(toEmit)) {
                std::cout << std::get<double>(toEmit);
            }
            else if (std::holds_alternative<int>(toEmit)) {
                std::cout << std::get<int>(toEmit);
            }
            else if (std::holds_alternative<std::string>(toEmit)) {
                std::cout << std::get<std::string>(toEmit);
            }
            else if (std::holds_alternative<bool>(toEmit)) {
                std::cout << std::get<bool>(toEmit);
            }

            std::cout << '\n';
        }
        else {
            throw std::runtime_error("Unknown statement");
        }
    }
}
    
