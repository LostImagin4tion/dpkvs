#include <gtest/gtest.h>

#include <dpkvs/engine/store_engine.h>

using NKVStore::NEngine::TStoreEngine;
using NKVStore::NEngine::TStorableValue;

class EngineInstantiatingTest
    : public testing::Test
{
protected:
    void SetUp() override {
        auto valueStr1 = std::string("hello");
        auto value1 = std::vector<uint8_t>(valueStr1.begin(), valueStr1.end());

        auto valueStr2 = std::string("world");
        auto value2 = std::vector<uint8_t>(valueStr2.begin(), valueStr2.end());

        defaultStore.Put(std::string(key1), TStorableValue(std::move(value1)));
        defaultStore.Put(std::string(key2), TStorableValue(std::move(value2)));

        auto valueStr3 = std::string("world");
        auto value3 = std::vector<uint8_t>(valueStr3.begin(), valueStr3.end());

        map[std::string(key1)] = TStorableValue(std::move(value3));
    }

    std::string key1 = "hello";
    std::string key2 = "world";
    std::unordered_map<std::string, TStorableValue> map;
    TStoreEngine defaultStore;
};

TEST_F(EngineInstantiatingTest, ParametrizedContructorTest) {
    auto mapSize = map.size();
    auto newStore = TStoreEngine(std::move(map));

    ASSERT_EQ(newStore.Size(), mapSize);
    ASSERT_TRUE(newStore.Get(key1).has_value());

    auto item = newStore.Get(key1).value().binaryData;
    auto itemStr = std::string(item.begin(), item.end());

    ASSERT_EQ(itemStr, "world");
}

TEST_F(EngineInstantiatingTest, MoveConstructorTest) {
    auto storeSize = defaultStore.Size();
    TStoreEngine newStore(std::move(defaultStore));

    ASSERT_EQ(newStore.Size(), storeSize);
    ASSERT_TRUE(newStore.Get(key1).has_value());
    ASSERT_TRUE(newStore.Get(key2).has_value());

    auto item = newStore.Get(key1).value().binaryData;
    auto itemStr = std::string(item.begin(), item.end());

    ASSERT_EQ(itemStr, "hello");
}

TEST_F(EngineInstantiatingTest, MoveAssignmentOperatorTest) {
    auto storeSize = defaultStore.Size();
    auto newStore = TStoreEngine(std::move(defaultStore));

    ASSERT_EQ(newStore.Size(), storeSize);
    ASSERT_TRUE(newStore.Get(key1).has_value());
    ASSERT_TRUE(newStore.Get(key2).has_value());

    auto item = newStore.Get(key2).value().binaryData;
    auto itemStr = std::string(item.begin(), item.end());

    ASSERT_EQ(itemStr, "world");
}
