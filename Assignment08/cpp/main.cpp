#include "file_handle.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4 ||
        (argc == 4 && std::string(argv[3]) != "--fail")) {
        std::cerr << "Usage: count_cpp PATH NEEDLE [--fail]\n";
        return 2;
    }
    try {
        const int count = count_matches_cpp(argv[1], argv[2], argc == 4);
        std::cout << "Matches: " << count << '\n';
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
