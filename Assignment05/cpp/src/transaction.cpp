#include "transaction.hpp"

Transaction::Transaction(Store& store)
    : store(store), backup(store), committed(false) {
}

void Transaction::commit() {
    committed = true;
}

Transaction::~Transaction() {
    if (!committed) {
        store.setData(backup.getData());
    }
}
