#include "file_handle.hpp"
#include <stdexcept>

FileHandle::FileHandle(const std::string& path)
    : file_(std::fopen(path.c_str(), "r")) {
    if (!file_) throw std::runtime_error("cannot open file: " + path);
    std::fprintf(stderr, "[C++] acquired %s\n", path.c_str());
}

FileHandle::~FileHandle() noexcept {
    std::fclose(file_);
    std::fprintf(stderr, "[C++] released file\n");
}

FILE* FileHandle::get() const noexcept { return file_; }

static bool read_line(FILE* file, std::string& line) {
    line.clear();
    int ch;
    while ((ch = std::fgetc(file)) != EOF) {
        if (ch == '\n') return true;
        line.push_back(static_cast<char>(ch));
    }
    if (std::ferror(file)) throw std::runtime_error("file read failed");
    return !line.empty(); 
}

int count_matches_cpp(const std::string& path, const std::string& needle,
                      bool fail_after_first_line) {
    if (needle.empty()) throw std::invalid_argument("empty search string");
    FileHandle file(path);
    std::string line;
    int count = 0;
    while (read_line(file.get(), line)) {
        if (line.find(needle) != std::string::npos) {
            ++count;
        }
        if (fail_after_first_line)
            throw std::runtime_error("simulated failure");
    }
    return count;
}
