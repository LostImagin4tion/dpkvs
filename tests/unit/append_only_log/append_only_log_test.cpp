#include <gtest/gtest.h>

#include <dpkvs/engine/storable_value.h>
#include <dpkvs/append_only_log/append_only_log.h>

using namespace NKVStore::NAppendLog;
using namespace NKVStore::NEngine;

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
    auto value1 = TStorableValue(std::vector<uint8_t>(valueStr1.begin(), valueStr1.end()));

    appendOnlyLog.AppendToLog(
        EAppendLogOperations::Put,
        key1,
        value1);

    // === Put second value ===

    auto key2 = std::string("darkness");
    auto valueStr2 = std::string("my old friend");
    auto value2 = TStorableValue(std::vector<uint8_t>(valueStr2.begin(), valueStr2.end()));

    appendOnlyLog.AppendToLog(
        EAppendLogOperations::Put,
        key2,
        value2);

    // === Recover ===

    auto recoveredEngine1 = appendOnlyLog.RecoverFromLog();

    auto recoveredValue1 = recoveredEngine1->Get(key1);
    auto recoveredValue2 = recoveredEngine1->Get(key2);

    ASSERT_TRUE(recoveredValue1.has_value());
    ASSERT_TRUE(recoveredValue2.has_value());

    auto recoveredStr1 = std::string(recoveredValue1->binaryData.begin(), recoveredValue1->binaryData.end());
    auto recoveredStr2 = std::string(recoveredValue2->binaryData.begin(), recoveredValue2->binaryData.end());

    ASSERT_EQ(recoveredStr1, valueStr1);
    ASSERT_EQ(recoveredStr2, valueStr2);

    // === Remove first value ===

    appendOnlyLog.AppendToLog(
        EAppendLogOperations::Remove,
        key1,
        value1);

    // === Recover again ===

    auto recoveredEngine2 = appendOnlyLog.RecoverFromLog();

    auto deletedValue1 = recoveredEngine2->Get(key1);
    auto recoveredAgainValue2 = recoveredEngine2->Get(key2);

    ASSERT_EQ(recoveredEngine2->Size(), 1);
    ASSERT_FALSE(deletedValue1.has_value());
    ASSERT_TRUE(recoveredAgainValue2.has_value());

    auto recoveredAgainStr2 = std::string(
        recoveredAgainValue2->binaryData.begin(),
        recoveredAgainValue2->binaryData.end());

    ASSERT_EQ(recoveredAgainStr2, valueStr2);
}
