#include <dpkvs/engine/store_engine.h>
#include <iostream>
#include <cassert>

using NKVStore::NEngine::TStoreEngine;

int main() {
    TStoreEngine store;

    store.Put("hello", std::string("world"));

    const auto& anyValue = store.Get("hello");

    auto value = std::any_cast<std::string>(anyValue);
    std::cout << value;
    assert(value == "world");

    return 0;
}
