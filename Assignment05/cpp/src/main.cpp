#include "store.hpp"

#include <iostream>
#include <sstream>
#include <string>

int main() {
    Store store;
    std::string line;

    std::cout << "Key-Value Store\n";
    std::cout << "Type HELP for commands.\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);

        std::stringstream input(line);
        std::string command;
        input >> command;

        if (command == "SET") {
            std::string key;
            std::string value;

            input >> key;
            std::getline(input, value);

            if (!value.empty() && value[0] == ' ') {
                value.erase(0, 1);
            }

            store.set(key, value);
        }

        else if (command == "GET") {
            std::string key;
            std::string value;

            input >> key;

            if (store.get(key, value)) {
                std::cout << value << std::endl;
            }
            else {
                std::cout << "Key not found\n";
            }
        }

        else if (command == "DELETE") {
            std::string key;
            input >> key;

            if (!store.remove(key)) {
                std::cout << "Key not found\n";
            }
        }

        else if (command == "LIST") {
            store.list(std::cout);
        }

        else if (command == "SAVE") {
            std::string filename;
            input >> filename;

            if (store.save(filename)) {
                std::cout << "Saved.\n";
            }
            else {
                std::cout << "Save failed.\n";
            }
        }

        else if (command == "LOAD") {
            std::string filename;
            input >> filename;

            if (store.load(filename)) {
                std::cout << "Loaded.\n";
            }
            else {
                std::cout << "Load failed.\n";
            }
        }

        else if (command == "HELP") {
            std::cout << "SET key value\n";
            std::cout << "GET key\n";
            std::cout << "DELETE key\n";
            std::cout << "LIST\n";
            std::cout << "SAVE filename\n";
            std::cout << "LOAD filename\n";
            std::cout << "QUIT\n";
        }

        else if (command == "QUIT") {
            break;
        }

        else {
            std::cout << "Unknown command\n";
        }
    }

    return 0;
}

