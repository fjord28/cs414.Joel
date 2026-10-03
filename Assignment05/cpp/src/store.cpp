#include "store.hpp"

#include <fstream>
#include <iostream>

void Store::set(const std::string& key, const std::string& value) {
    data[key] = value;
}

bool Store::get(const std::string& key, std::string& value) const {
    auto it = data.find(key);

    if (it == data.end()) {
        return false;
    }

    value = it->second;
    return true;
}

bool Store::remove(const std::string& key) {
    return data.erase(key) > 0;
}

void Store::list(std::ostream& out) const {
    for (const auto& item : data) {
        out << item.first << " = " << item.second << std::endl;
    }
}

bool Store::save(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file) {
        return false;
    }

    for (const auto& item : data) {
        file << item.first << '\t' << item.second << '\n';
    }

    return true;
}

bool Store::load(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        return false;
    }

    data.clear();

    std::string key;
    std::string value;

    while (std::getline(file, key, '\t')) {
        if (std::getline(file, value)) {
            data[key] = value;
        }
    }

    return true;
}

std::map<std::string, std::string> Store::getData() const {
    return data;
}

void Store::setData(const std::map<std::string, std::string>& newData) {
    data = newData;
}
