#pragma once

#include "store.hpp"

class Transaction {
private:
    Store& store;
    Store backup;
    bool committed;

public:
    Transaction(Store& store);

    void commit();

    ~Transaction();
};
