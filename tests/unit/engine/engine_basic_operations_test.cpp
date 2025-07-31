#include <gtest/gtest.h>

#include <dpkvs/engine/store_engine.h>

using NKVStore::NEngine::TStoreEngine;
using NKVStore::NEngine::TStorableValue;

class EngineBasicOperationsTest
    : public testing::Test
{
protected:
    TStoreEngine store;
};

TEST_F(EngineBasicOperationsTest, StoreBasicOperations) {
    ASSERT_EQ(store.Size(), 0);

    // === first put ===

    auto key1 = std::string("hello");
    auto valueStr1 = std::string("world");
    auto value1 = std::vector<uint8_t>(valueStr1.begin(), valueStr1.end());

    store.Put(
        std::move(key1),
        TStorableValue(std::move(value1)));

    ASSERT_EQ(store.Size(), 1);
    ASSERT_TRUE(store.Get("hello").has_value());

    auto binaryData1 = store.Get("hello")
        .value()
        .binaryData;
    auto string = std::string(binaryData1.begin(), binaryData1.end());

    ASSERT_EQ(string, "world");

    // === second put ===

    auto key2 = std::string("world");
    auto valueStr2 = std::string("hello");
    auto value2 = std::vector<uint8_t>(valueStr2.begin(), valueStr2.end());

    store.Put(
        std::move(key2),
        TStorableValue(std::move(value2)));

    ASSERT_EQ(store.Size(), 2);

    // === removals ===

    auto removed1 = store.Remove("hello");

    ASSERT_TRUE(removed1);
    ASSERT_EQ(store.Size(), 1);

    auto removed2 = store.Remove("world");

    ASSERT_TRUE(removed2);
    ASSERT_EQ(store.Size(), 0);
}
