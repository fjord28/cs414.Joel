#pragma once

#include <string>
#include <stdexcept>
#include "tokenizer.hpp"

struct CommandNode {
    TokenType type;
    std::string path;
};

class Parser {
public:
    explicit Parser(Tokenizer& tokenizer)
        : tokenizer(tokenizer) {
    }

    void parse() {
        parseCommand();

        if (tokenizer.currentToken().type != TokenType::END) {
            throw std::runtime_error("Syntax error: unexpected token");
        }
    }
    CommandNode getCommand() const {
    return command;
}

private:
    Tokenizer& tokenizer;
    CommandNode command;

    void parseCommand() {
        if (tokenizer.currentToken().type == TokenType::LS) {
            parseLs();
        } else if (tokenizer.currentToken().type == TokenType::CD) {
            parseCd();
        } else if (tokenizer.currentToken().type == TokenType::CAT) {
            parseCat();
        } else if (tokenizer.currentToken().type == TokenType::PRINT) {
            parsePrint();
        } else if (tokenizer.currentToken().type == TokenType::EXEC) {
            parseExec();
        } else {
            throw std::runtime_error("Syntax error: expected command");
        }
    }

void parseLs() {
    command.type = TokenType::LS;
    tokenizer.nextToken();

    if (tokenizer.currentToken().type == TokenType::BACKSLASH ||
        tokenizer.currentToken().type == TokenType::IDENTIFIER) {
        parsePath();
    }
}
void parseCd() {
    command.type = TokenType::CD;
    tokenizer.nextToken();

    if (tokenizer.currentToken().type == TokenType::BACKSLASH ||
        tokenizer.currentToken().type == TokenType::IDENTIFIER) {
        parsePath();
    }
}
void parseCat() {
    command.type = TokenType::CAT;
    tokenizer.nextToken();

    if (tokenizer.currentToken().type == TokenType::BACKSLASH ||
        tokenizer.currentToken().type == TokenType::IDENTIFIER) {
        parseFilePath();
    } else {
        throw std::runtime_error("Syntax error: expected file path");
    }
}
void parseFilePath() {
    command.path = "";

if (tokenizer.currentToken().type == TokenType::BACKSLASH) {
    command.path += "\\";
    tokenizer.nextToken();
}

    while (tokenizer.currentToken().type == TokenType::IDENTIFIER) {
        command.path += tokenizer.currentToken().value;
        tokenizer.nextToken();

if (tokenizer.currentToken().type == TokenType::DOT) {
    command.path += ".";
    tokenizer.nextToken();

    if (tokenizer.currentToken().type != TokenType::IDENTIFIER) {
        throw std::runtime_error("Syntax error: expected file extension");
    }

    command.path += tokenizer.currentToken().value;
    tokenizer.nextToken();
    return;
}

 if (tokenizer.currentToken().type == TokenType::BACKSLASH) {
    command.path += "\\";
    tokenizer.nextToken();
} else {
            throw std::runtime_error("Syntax error: expected '.' or '\\'");
        }
    }

    throw std::runtime_error("Syntax error: expected file name");
}
void parsePrint() {
    command.type = TokenType::PRINT;
    tokenizer.nextToken();

    if (tokenizer.currentToken().type == TokenType::BACKSLASH ||
        tokenizer.currentToken().type == TokenType::IDENTIFIER) {
        parseFilePath();
    } else {
        throw std::runtime_error("Syntax error: expected file path");
    }
}

    void parsePath() {
    command.path = "";

    if (tokenizer.currentToken().type == TokenType::BACKSLASH) {
        command.path += "\\";
        tokenizer.nextToken();

        if (tokenizer.currentToken().type == TokenType::END) {
            return;
        }
    }

    if (tokenizer.currentToken().type != TokenType::IDENTIFIER) {
        throw std::runtime_error("Syntax error: expected folder name");
    }

    command.path += tokenizer.currentToken().value;
    tokenizer.nextToken();

    while (tokenizer.currentToken().type == TokenType::BACKSLASH) {
        command.path += "\\";
        tokenizer.nextToken();

        if (tokenizer.currentToken().type != TokenType::IDENTIFIER) {
            throw std::runtime_error("Syntax error: expected folder name");
        }

        command.path += tokenizer.currentToken().value;
        tokenizer.nextToken();
    }
}
    void parseExec() {
    command.type = TokenType::EXEC;
    tokenizer.nextToken();

    if (tokenizer.currentToken().type == TokenType::BACKSLASH ||
        tokenizer.currentToken().type == TokenType::IDENTIFIER) {
        parseFilePath();
    } else {
        throw std::runtime_error("Syntax error: expected file path");
    }
}
};
