#include <iostream>
#include <string>
#include "Parser.hpp"

int main() {
    std::string input = "set $x = $a + $b * $c";

    Tokenizer tokenizer(input);
    Parser parser(tokenizer);

    try {
        parser.parse();
        std::cout << "Parse successful" << std::endl;
    } catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}
