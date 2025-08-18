#include <gtest/gtest.h>

#include <dpkvs/core/engines/hash_map/engine/store_engine.h>

using NKVStore::NCore::NEngine::THashMapStoreEngine;

class EngineInstantiatingTest
    : public testing::Test
{
protected:
    void SetUp() override {
        auto valueStr1 = std::string("hello");
        auto valueStr2 = std::string("world");

        TStoreRecord record1;
        record1.set_data(std::move(valueStr1));
        TStoreRecord record2;
        record2.set_data(std::move(valueStr2));

        defaultStore.Put(std::string(key1), std::move(record1));
        defaultStore.Put(std::string(key2), std::move(record2));

        auto valueStr3 = std::string("world");
        auto record3 = std::make_shared<TStoreRecord>();
        record3->set_data(std::move(valueStr3));
        map[std::string(key1)] = std::move(record3);
    }

    std::string key1 = "hello";
    std::string key2 = "world";
    std::unordered_map<std::string, TStoreRecordPtr> map;
    THashMapStore defaultStore;
};

TEST_F(EngineInstantiatingTest, ParametrizedContructorTest) {
    auto mapSize = map.size();
    auto newStore = THashMapStore(std::move(map));

    ASSERT_EQ(newStore.Size(), mapSize);
    ASSERT_TRUE(newStore.Get(key1));

    auto item = newStore.Get(key1)->data();
    ASSERT_EQ(item, "world");
}

TEST_F(EngineInstantiatingTest, MoveConstructorTest) {
    auto storeSize = defaultStore.Size();
    THashMapStore newStore(std::move(defaultStore));

    ASSERT_EQ(newStore.Size(), storeSize);
    ASSERT_TRUE(newStore.Get(key1));
    ASSERT_TRUE(newStore.Get(key2));

    auto item = newStore.Get(key1)->data();
    ASSERT_EQ(item, "hello");
}

TEST_F(EngineInstantiatingTest, MoveAssignmentOperatorTest) {
    auto storeSize = defaultStore.Size();
    auto newStore = THashMapStore(std::move(defaultStore));

    ASSERT_EQ(newStore.Size(), storeSize);
    ASSERT_TRUE(newStore.Get(key1));
    ASSERT_TRUE(newStore.Get(key2));

    auto item = newStore.Get(key2)->data();
    ASSERT_EQ(item, "world");
}
