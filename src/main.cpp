#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "interpreter.hpp"
#include "parser.hpp"
#include "lexer.hpp"

std::string readFile(std::string fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        throw std::runtime_error("Couldn't open the file");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int args, char* argv[]) {
    if (args < 2) {
        std::cerr << "Usage: " << argv[0] << " <plik.mr>\n";
        return 1;
    }

    std::string c = readFile(argv[1]);

    Lexer lexer(c);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    std::vector<std::unique_ptr<Statement>> parsed = parser.parse();

    Interpreter interpreter;
    interpreter.execute(parsed);
    
    return 0;
}