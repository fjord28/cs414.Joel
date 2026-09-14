#pragma once

#include <string>
#include <cctype>
#include <stdexcept>

enum class TokenType {
    LS,
    CD,
    CAT,
    PRINT,
    EXEC,
    IDENTIFIER,
    DOT,
    BACKSLASH,
    END
};

struct Token {
    TokenType type;
    std::string value;
};

class Tokenizer {
public:
    explicit Tokenizer(const std::string& input)
    :input(input), pos(0) {
        nextToken();
    }
    void nextToken() {
    while (pos < input.size() && isspace(input[pos])) {
        pos++;
    }

    if (pos == input.size()) {
        current = {TokenType::END, ""};
        return;
    }

    char c = input[pos];

    if (isalpha(c)) {
        size_t start = pos;

        while (pos < input.size() && isalnum(input[pos])) {
            pos++;
        }

        std::string word = input.substr(start, pos - start);

        if (word == "ls") {
            current = {TokenType::LS, word};
        } else if (word == "cd") {
            current = {TokenType::CD, word};
        } else if (word == "cat") {
            current = {TokenType::CAT, word};
        } else if (word == "print") {
            current = {TokenType::PRINT, word};
        } else if (word == "exec") {
            current = {TokenType::EXEC, word};
        } else {
            current = {TokenType::IDENTIFIER, word};
        }

    } else if (c == '.') {
        current = {TokenType::DOT, "."};
        pos++;

    } else if (c == '\\') {
        current = {TokenType::BACKSLASH, "\\"};
        pos++;

    } else {
        throw std::runtime_error("Unexpected character");
    }
}
    Token currentToken() const {
        return current;
    }
private:
    std::string input;
    size_t pos;
    Token current;
};
