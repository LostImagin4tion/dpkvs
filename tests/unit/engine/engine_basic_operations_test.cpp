#include <gtest/gtest.h>

#include <dpkvs/core/engines/hash_map/engine/store_engine.h>

using NKVStore::NCore::NEngine::THashMapStoreEngine;
using NKVStore::NCore::NRecord::TStoreValue;

class EngineBasicOperationsTest
    : public testing::Test
{
protected:
    THashMapStore store;
};

TEST_F(EngineBasicOperationsTest, StoreBasicOperations) {
    ASSERT_EQ(store.Size(), 0);

    // === first put ===

    auto key1 = std::string("hello");
    auto valueStr1 = std::string("world");

    TStoreValue record1;
    record1.set_data(std::move(valueStr1));
    store.Put(std::move(key1), std::move(record1));

    ASSERT_EQ(store.Size(), 1);
    ASSERT_TRUE(store.Get("hello"));

    auto string = store.Get("hello")->data();
    ASSERT_EQ(string, "world");

    // === second put ===

    auto key2 = std::string("world");
    auto valueStr2 = std::string("hello");

    TStoreValue record2;
    record2.set_data(std::move(valueStr2));
    store.Put(std::move(key2),std::move(record2));

    ASSERT_EQ(store.Size(), 2);

    // === removals ===

    auto removed1 = store.Remove("hello");

    ASSERT_TRUE(removed1);
    ASSERT_EQ(store.Size(), 1);

    auto removed2 = store.Remove("world");

    ASSERT_TRUE(removed2);
    ASSERT_EQ(store.Size(), 0);
}
