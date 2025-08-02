#include <gtest/gtest.h>

#include <dpkvs/engine/storable_value.h>
#include <dpkvs/append_only_log/log_serializer.h>

using namespace NKVStore::NEngine;
using namespace NKVStore::NAppendLog;

TEST(SerializerTest, WriteReadLogsTest) {
    auto logSerializer = TAppendLogSerializer();

    // === Put first value ===

    auto key1 = std::string("hello");
    auto valueStr1 = std::string("world");
    auto storableValue1 = TStorableValue(std::vector<uint8_t>(valueStr1.begin(), valueStr1.end()));

    logSerializer.WritePutLog(key1, storableValue1);

    // === Put second value ===

    auto key2 = std::string("darkness");
    auto valueStr2 = std::string("my old friend");
    auto storableValue2 = TStorableValue(std::vector<uint8_t>(valueStr2.begin(), valueStr2.end()));

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

    auto readValue1 = logSerializer.ReadValue().binaryData;
    auto readStr1 =  std::string(readValue1.begin(), readValue1.end());
    ASSERT_EQ(readStr1, valueStr1);

    // === Read put second value log ===

    EAppendLogOperations command2 = logSerializer.ReadCommand();
    ASSERT_EQ(command2, EAppendLogOperations::Put);

    auto readKey2 = logSerializer.ReadKey();
    ASSERT_EQ(readKey2, key2);

    auto readValue2 = logSerializer.ReadValue().binaryData;
    auto readStr2 =  std::string(readValue2.begin(), readValue2.end());
    ASSERT_EQ(readStr2, valueStr2);

    // === Read removed first value log ===

    EAppendLogOperations command3 = logSerializer.ReadCommand();
    ASSERT_EQ(command3, EAppendLogOperations::Remove);

    auto readKey3 = logSerializer.ReadKey();
    ASSERT_EQ(readKey3, key1);
}
