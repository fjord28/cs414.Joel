#pragma once
#include <cstdio>
#include <string>

class FileHandle {
public:
    explicit FileHandle(const std::string& path);
    ~FileHandle() noexcept;
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    FileHandle(FileHandle&&) = delete;
    FileHandle& operator=(FileHandle&&) = delete;
    FILE* get() const noexcept;
private:
    FILE* file_ = nullptr;
};

int count_matches_cpp(const std::string& path, const std::string& needle,
                      bool fail_after_first_line = false);
