#include "factory.h"

namespace NKVStore::NCore::NRecord
{

TStoreRecord CreatePutRecord(const std::string& key, const TStoreValue& value) {
    TStoreRecord record;
    auto operation = record.mutable_put_operation();
    operation->set_key(key);
    *(operation->mutable_value()) = value;

    return record;
}

TStoreRecord CreateRemoveRecord(const std::string& key) {
    TStoreRecord record;
    auto operation = record.mutable_remove_operation();
    operation->set_key(key);

    return record;
}

} // namespace NKVStore::NCore::NRecord
