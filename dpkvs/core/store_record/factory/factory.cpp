#include "factory.h"

namespace NKVStore::NCore::NRecord
{

TStoreRecord CreatePutRecord(
    const std::string& key,
    const TStoreValue& value,
    uint64_t lsn)
{
    TStoreRecord record;
    record.set_log_sequence_number(lsn);

    auto operation = record.mutable_put_operation();
    operation->set_key(key);
    *operation->mutable_value() = value;

    return record;
}

TStoreRecord CreateRemoveRecord(const std::string& key, uint64_t lsn)
{
    TStoreRecord record;
    record.set_log_sequence_number(lsn);
    record.mutable_remove_operation()->set_key(key);

    return record;
}

} // namespace NKVStore::NCore::NRecord
