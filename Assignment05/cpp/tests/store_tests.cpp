#include "store.hpp"
#include "transaction.hpp"

#include <cassert>
#include <string>

int main() {
    Store store;

    store.set("x", "10");

    std::string value;

    assert(store.get("x", value));
    assert(value == "10");

    assert(!store.get("missing", value));

    store.set("y", "20");

    assert(store.remove("y"));
    assert(!store.get("y", value));

    {
        Transaction tx(store);

        store.set("x", "20");
        store.set("y", "30");
    }

    assert(store.get("x", value));
    assert(value == "10");

    assert(!store.get("y", value));

    {
        Transaction tx(store);

        store.set("x", "20");
        store.set("y", "30");

        tx.commit();
    }

    assert(store.get("x", value));
    assert(value == "20");

    assert(store.get("y", value));
    assert(value == "30");

    return 0;
}
