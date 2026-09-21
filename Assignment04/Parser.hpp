#pragma once

#include <string>
#include <map>
#include <stdexcept>
#include "Tokenizer.hpp"

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

        printSymbolTable();
    }

    CommandNode getCommand() const {
        return command;
    }

private:
    Tokenizer& tokenizer;
    CommandNode command;
    std::map<std::string, std::string> symbolTable;

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
        } else if (tokenizer.currentToken().type == TokenType::SET) {
            parseSet();
        } else if (tokenizer.currentToken().type == TokenType::ECHO) {
            parseEcho();
        } else {
            throw std::runtime_error("Syntax error: expected command");
        }
    }

    void parseLs() {
        command.type = TokenType::LS;
        tokenizer.nextToken();

        if (tokenizer.currentToken().type == TokenType::BACKSLASH ||
            tokenizer.currentToken().type == TokenType::IDENTIFIER ||
            tokenizer.currentToken().type == TokenType::VARIABLE) {
            parsePath();
        }
    }

    void parseCd() {
        command.type = TokenType::CD;
        tokenizer.nextToken();

        if (tokenizer.currentToken().type == TokenType::BACKSLASH ||
            tokenizer.currentToken().type == TokenType::IDENTIFIER ||
            tokenizer.currentToken().type == TokenType::VARIABLE) {
            parsePath();
        }
    }

    void parseCat() {
        command.type = TokenType::CAT;
        tokenizer.nextToken();

        if (tokenizer.currentToken().type == TokenType::BACKSLASH ||
            tokenizer.currentToken().type == TokenType::IDENTIFIER ||
            tokenizer.currentToken().type == TokenType::VARIABLE) {
            parseFilePath();
        } else {
            throw std::runtime_error("Syntax error: expected file path");
        }
    }

    void parsePrint() {
        command.type = TokenType::PRINT;
        tokenizer.nextToken();

        if (tokenizer.currentToken().type == TokenType::BACKSLASH ||
            tokenizer.currentToken().type == TokenType::IDENTIFIER ||
            tokenizer.currentToken().type == TokenType::VARIABLE) {
            parseFilePath();
        } else {
            throw std::runtime_error("Syntax error: expected file path");
        }
    }

    void parseExec() {
        command.type = TokenType::EXEC;
        tokenizer.nextToken();

        if (tokenizer.currentToken().type == TokenType::BACKSLASH ||
            tokenizer.currentToken().type == TokenType::IDENTIFIER ||
            tokenizer.currentToken().type == TokenType::VARIABLE) {
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

        while (tokenizer.currentToken().type == TokenType::IDENTIFIER ||
               tokenizer.currentToken().type == TokenType::VARIABLE) {

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

    void parsePath() {
        command.path = "";

        if (tokenizer.currentToken().type == TokenType::BACKSLASH) {
            command.path += "\\";
            tokenizer.nextToken();

            if (tokenizer.currentToken().type == TokenType::END) {
                return;
            }
        }

        if (tokenizer.currentToken().type != TokenType::IDENTIFIER &&
            tokenizer.currentToken().type != TokenType::VARIABLE) {
            throw std::runtime_error("Syntax error: expected folder name");
        }

        command.path += tokenizer.currentToken().value;
        tokenizer.nextToken();

        while (tokenizer.currentToken().type == TokenType::BACKSLASH) {
            command.path += "\\";
            tokenizer.nextToken();

            if (tokenizer.currentToken().type != TokenType::IDENTIFIER &&
                tokenizer.currentToken().type != TokenType::VARIABLE) {
                throw std::runtime_error("Syntax error: expected folder name");
            }

            command.path += tokenizer.currentToken().value;
            tokenizer.nextToken();
        }
    }

    void parseSet() {
        tokenizer.nextToken();

        if (tokenizer.currentToken().type != TokenType::VARIABLE) {
            throw std::runtime_error("Syntax error: expected variable");
        }

        std::string variable = tokenizer.currentToken().value;

        tokenizer.nextToken();

        if (tokenizer.currentToken().type != TokenType::EQUALS) {
            throw std::runtime_error("Syntax error: expected '='");
        }

        tokenizer.nextToken();

        parseExpression();

        symbolTable[variable] = "expression";
    }

    void parseEcho() {
        tokenizer.nextToken();

        if (tokenizer.currentToken().type != TokenType::VARIABLE) {
            throw std::runtime_error("Syntax error: expected variable");
        }

        std::string variable = tokenizer.currentToken().value;

        if (symbolTable.find(variable) == symbolTable.end()) {
            symbolTable[variable] = "";
        }

        tokenizer.nextToken();
    }

    void parseExpression() {
        parseTerm();

        while (tokenizer.currentToken().type == TokenType::PLUS ||
               tokenizer.currentToken().type == TokenType::MINUS) {
            tokenizer.nextToken();
            parseTerm();
        }
    }

    void parseTerm() {
        parseFactor();

        while (tokenizer.currentToken().type == TokenType::MULTIPLY ||
               tokenizer.currentToken().type == TokenType::DIVIDE) {
            tokenizer.nextToken();
            parseFactor();
        }
    }

    void parseFactor() {
        if (tokenizer.currentToken().type == TokenType::VARIABLE ||
            tokenizer.currentToken().type == TokenType::IDENTIFIER) {

            tokenizer.nextToken();

        } else if (tokenizer.currentToken().type == TokenType::LEFT_PAREN) {

            tokenizer.nextToken();
            parseExpression();

            if (tokenizer.currentToken().type != TokenType::RIGHT_PAREN) {
                throw std::runtime_error("Syntax error: expected ')'");
            }

            tokenizer.nextToken();

        } else {
            throw std::runtime_error("Syntax error: expected expression");
        }
    }

    void printSymbolTable() {
        std::cout << "Symbol Table:" << std::endl;

        for (const auto& entry : symbolTable) {
            std::cout << entry.first << " = " << entry.second << std::endl;
        }
    }
};
