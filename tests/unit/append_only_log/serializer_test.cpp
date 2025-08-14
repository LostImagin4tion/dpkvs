#include <gtest/gtest.h>

#include <dpkvs/engine/storable_value.h>
#include <dpkvs/append_only_log/log_serializer.h>

using NKVStore::NEngine::TStorableValue;
using NKVStore::NAppendLog::TAppendLogSerializer;
using NKVStore::NAppendLog::EAppendLogOperations;

TEST(SerializerTest, WriteReadLogsTest) {
    auto logSerializer = TAppendLogSerializer();

    // === Put first value ===

    auto key1 = std::string("hello");
    auto valueStr1 = std::string("world");
    auto storableValue1 = TStorableValue(valueStr1);

    logSerializer.WritePutLog(key1, storableValue1);

    // === Put second value ===

    auto key2 = std::string("darkness");
    auto valueStr2 = std::string("my old friend");
    auto storableValue2 = TStorableValue(valueStr2);

    logSerializer.WritePutLog(key2, storableValue2);

    // === Remove first value ===

    logSerializer.WriteRemoveLog(key1);

    // === Read put first value log ===
    logSerializer.EnableReadMode();

    ASSERT_TRUE(logSerializer.ReadyToRead());

    EAppendLogOperations command1 = logSerializer.ReadCommand();
    ASSERT_EQ(command1, EAppendLogOperations::Put);

    auto readKey1 = logSerializer.ReadKey();
    ASSERT_EQ(readKey1, key1);

    auto readStr1 =  logSerializer.ReadValue().data;
    ASSERT_EQ(readStr1, valueStr1);

    // === Read put second value log ===

    EAppendLogOperations command2 = logSerializer.ReadCommand();
    ASSERT_EQ(command2, EAppendLogOperations::Put);

    auto readKey2 = logSerializer.ReadKey();
    ASSERT_EQ(readKey2, key2);

    auto readStr2 =  logSerializer.ReadValue().data;
    ASSERT_EQ(readStr2, valueStr2);

    // === Read removed first value log ===

    EAppendLogOperations command3 = logSerializer.ReadCommand();
    ASSERT_EQ(command3, EAppendLogOperations::Remove);

    auto readKey3 = logSerializer.ReadKey();
    ASSERT_EQ(readKey3, key1);
}
