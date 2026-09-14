#include <iostream>
#include <string>
#include "parser.hpp"

int main() {
    std::string input = "cat Documents\\School\\REPORT.TXT";

    Tokenizer tokenizer(input);
    Parser parser(tokenizer);

    try {
        parser.parse();

        CommandNode command = parser.getCommand();

        if (command.type == TokenType::LS) {
            std::cout << "LS";
        } else if (command.type == TokenType::CD) {
            std::cout << "CD";
        } else if (command.type == TokenType::CAT) {
            std::cout << "CAT";
        } else if (command.type == TokenType::PRINT) {
            std::cout << "PRINT";
        } else if (command.type == TokenType::EXEC) {
            std::cout << "EXEC";
        }

        std::cout << " " << command.path << std::endl;

    } catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}
