#include <gtest/gtest.h>

#include <dpkvs/core/store_record/store_record.h>
#include <dpkvs/core/engines/hash_map/append_only_log/log_serializer.h>

using NKVStore::NCore::NEngine::NAppendLog::TAppendLogSerializer;
using NKVStore::NCore::NEngine::EStoreEngineOperations;

TEST(SerializerTest, WriteReadLogsTest) {
    auto logSerializer = TAppendLogSerializer();

    // === Put first value ===

    auto key1 = std::string("hello");
    auto valueStr1 = std::string("world");
    auto storableValue1 = TStoreRecord(valueStr1);

    logSerializer.WritePutLog(key1, storableValue1);

    // === Put second value ===

    auto key2 = std::string("darkness");
    auto valueStr2 = std::string("my old friend");
    auto storableValue2 = TStoreRecord(valueStr2);

    logSerializer.WritePutLog(key2, storableValue2);

    // === Remove first value ===

    logSerializer.WriteRemoveLog(key1);

    // === Read put first value log ===
    logSerializer.EnableReadMode();

    ASSERT_TRUE(logSerializer.ReadyToRead());

    auto command1 = logSerializer.ReadCommand();
    ASSERT_EQ(command1, EStoreEngineOperations::Put);

    auto readKey1 = logSerializer.ReadKey();
    ASSERT_EQ(readKey1, key1);

    auto readStr1 =  logSerializer.ReadValue().data;
    ASSERT_EQ(readStr1, valueStr1);

    // === Read put second value log ===

    auto command2 = logSerializer.ReadCommand();
    ASSERT_EQ(command2, EStoreEngineOperations::Put);

    auto readKey2 = logSerializer.ReadKey();
    ASSERT_EQ(readKey2, key2);

    auto readStr2 =  logSerializer.ReadValue().data;
    ASSERT_EQ(readStr2, valueStr2);

    // === Read removed first value log ===

    auto command3 = logSerializer.ReadCommand();
    ASSERT_EQ(command3, EStoreEngineOperations::Remove);

    auto readKey3 = logSerializer.ReadKey();
    ASSERT_EQ(readKey3, key1);
}
