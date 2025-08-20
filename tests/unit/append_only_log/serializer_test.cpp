#include <gtest/gtest.h>

#include <dpkvs/core/store_value/generated/store_value.pb.h>
#include <dpkvs/core/store_record/serializer/record_serializer.h>
#include <dpkvs/core/store_record/factory/factory.h>

using NKVStore::NCore::NRecord::TStoreValue;
using NKVStore::NCore::NRecord::TStoreRecordSerializer;
using NKVStore::NCore::NRecord::TStoreRecord;
using NKVStore::NCore::NRecord::CreatePutRecord;
using NKVStore::NCore::NRecord::CreateRemoveRecord;

TEST(SerializerTest, WriteReadLogsTest) {
    auto logSerializer = TStoreRecordSerializer();

    // === Put first value ===

    auto key1 = std::string("hello");
    auto valueStr1 = std::string("world");
    TStoreValue storableValue1;
    storableValue1.set_data(valueStr1);
    storableValue1.set_flags(1);

    auto time = std::chrono::system_clock::now() + std::chrono::hours(24);
    storableValue1.set_expiry_millis(time.time_since_epoch().count());

    auto record1 = CreatePutRecord(key1, storableValue1, 0);
    logSerializer.WriteRecord(record1);

    // === Put second value ===

    auto key2 = std::string("darkness");
    auto valueStr2 = std::string("my old friend");
    TStoreValue storableValue2;
    storableValue2.set_data(valueStr2);

    auto record2 = CreatePutRecord(key2, storableValue2, 1);
    logSerializer.WriteRecord(record2);

    // === Remove first value ===

    auto record3 = CreateRemoveRecord(key1, 2);
    logSerializer.WriteRecord(record3);

    // === Read record by record ===
    logSerializer.EnableReadMode();

    TStoreRecord readRecord1;
    ASSERT_TRUE(logSerializer.ReadRecord(readRecord1));
    ASSERT_TRUE(readRecord1.has_put_operation());
    ASSERT_EQ(readRecord1.put_operation().key(), key1);
    ASSERT_EQ(readRecord1.put_operation().value().data(), valueStr1);

    TStoreRecord readRecord2;
    ASSERT_TRUE(logSerializer.ReadRecord(readRecord2));
    ASSERT_TRUE(readRecord2.has_put_operation());
    ASSERT_EQ(readRecord2.put_operation().key(), key2);
    ASSERT_EQ(readRecord2.put_operation().value().data(), valueStr2);

    TStoreRecord readRecord3;
    ASSERT_TRUE(logSerializer.ReadRecord(readRecord3));
    ASSERT_TRUE(readRecord3.has_remove_operation());
    ASSERT_EQ(readRecord3.remove_operation().key(), key1);
}
