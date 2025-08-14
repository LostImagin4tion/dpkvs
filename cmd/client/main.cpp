#include <dpkvs/store_controller/store_controller.h>
#include <iostream>
#include <cassert>

using NKVStore::NController::TStoreController;

int main() {
    TStoreController store;

    std::string key = "hello";
    std::string value = "world";

    auto valueBytes = std::vector<uint8_t>(value.begin(), value.end());

    store.Put(key, valueBytes);

    const auto& anyValue = store.Get("hello");

    assert(anyValue.has_value());

    return 0;
}
