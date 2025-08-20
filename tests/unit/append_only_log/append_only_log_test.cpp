#include <gtest/gtest.h>

#include <dpkvs/core/engines/hash_map/persistence/append_only_log.h>

using NKVStore::NCore::NEngine::NPersistence::TAppendOnlyLog;
using NKVStore::NCore::NRecord::TStoreValue;

class AppendOnlyLogTest
    : public testing::Test
{
protected:
};

TEST_F(AppendOnlyLogTest, AppendToLogsAndRecoverTest) {
    auto appendOnlyLog = TAppendOnlyLog();

    // === Put first value ===

    auto key1 = std::string("hello");
    auto valueStr1 = std::string("world");
    TStoreValue value1;
    value1.set_data(valueStr1);

    appendOnlyLog.AppendPutOperation(key1, value1);

    // === Put second value ===

    auto key2 = std::string("darkness");
    auto valueStr2 = std::string("my old friend");
    TStoreValue value2;
    value2.set_data(valueStr2);

    appendOnlyLog.AppendPutOperation(key2,value2);

    // === Recover ===

    auto recoveredEngine1 = appendOnlyLog.RecoverFromLog();

    auto recoveredValue1 = recoveredEngine1->Get(key1);
    auto recoveredValue2 = recoveredEngine1->Get(key2);

    ASSERT_TRUE(recoveredValue1);
    ASSERT_TRUE(recoveredValue2);

    auto recoveredStr1 = recoveredValue1->data();
    auto recoveredStr2 = recoveredValue2->data();

    ASSERT_EQ(recoveredStr1, valueStr1);
    ASSERT_EQ(recoveredStr2, valueStr2);

    // === Remove first value ===

    appendOnlyLog.AppendRemoveOperation(key1);

    // === Recover again ===

    auto recoveredEngine2 = appendOnlyLog.RecoverFromLog();

    auto deletedValue1 = recoveredEngine2->Get(key1);
    auto recoveredAgainValue2 = recoveredEngine2->Get(key2);

    ASSERT_EQ(recoveredEngine2->Size(), 1);
    ASSERT_FALSE(deletedValue1);
    ASSERT_TRUE(recoveredAgainValue2);

    auto recoveredAgainStr2 = recoveredAgainValue2->data();

    ASSERT_EQ(recoveredAgainStr2, valueStr2);
}
