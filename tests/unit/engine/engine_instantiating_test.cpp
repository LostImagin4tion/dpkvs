#include <gtest/gtest.h>

#include <dpkvs/core/engines/hash_map/hash_map_store_engine.h>

using NKVStore::NCore::NEngine::THashMapEngine;

class EngineInstantiatingTest
    : public testing::Test
{
protected:
    void SetUp() override {
        auto valueStr1 = std::string("hello");
        auto valueStr2 = std::string("world");

        defaultStore.Put(std::string(key1), TStoreRecord(std::move(valueStr1)));
        defaultStore.Put(std::string(key2), TStoreRecord(std::move(valueStr2)));

        auto valueStr3 = std::string("world");
        map[std::string(key1)] = std::make_shared<TStoreRecord>(std::move(valueStr3));
    }

    std::string key1 = "hello";
    std::string key2 = "world";
    std::unordered_map<std::string, TStoreRecordPtr> map;
    THashMapEngine defaultStore;
};

TEST_F(EngineInstantiatingTest, ParametrizedContructorTest) {
    auto mapSize = map.size();
    auto newStore = THashMapEngine(std::move(map));

    ASSERT_EQ(newStore.Size(), mapSize);
    ASSERT_TRUE(newStore.Get(key1));

    auto item = newStore.Get(key1)->data;
    ASSERT_EQ(item, "world");
}

TEST_F(EngineInstantiatingTest, MoveConstructorTest) {
    auto storeSize = defaultStore.Size();
    THashMapEngine newStore(std::move(defaultStore));

    ASSERT_EQ(newStore.Size(), storeSize);
    ASSERT_TRUE(newStore.Get(key1));
    ASSERT_TRUE(newStore.Get(key2));

    auto item = newStore.Get(key1)->data;
    ASSERT_EQ(item, "hello");
}

TEST_F(EngineInstantiatingTest, MoveAssignmentOperatorTest) {
    auto storeSize = defaultStore.Size();
    auto newStore = THashMapEngine(std::move(defaultStore));

    ASSERT_EQ(newStore.Size(), storeSize);
    ASSERT_TRUE(newStore.Get(key1));
    ASSERT_TRUE(newStore.Get(key2));

    auto item = newStore.Get(key2)->data;
    ASSERT_EQ(item, "world");
}
