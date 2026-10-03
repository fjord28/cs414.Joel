#pragma once

#include <map>
#include <string>
#include <ostream>

class Store {
private:
    std::map<std::string, std::string> data;

public:
    void set(const std::string& key, const std::string& value);
    bool get(const std::string& key, std::string& value) const;
    bool remove(const std::string& key);
    void list(std::ostream& out) const;

    bool save(const std::string& filename) const;
    bool load(const std::string& filename);

    std::map<std::string, std::string> getData() const;
    void setData(const std::map<std::string, std::string>& newData);
};
