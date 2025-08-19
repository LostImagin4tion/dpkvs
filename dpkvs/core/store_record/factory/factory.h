#pragma once

#include "dpkvs/core/store_value/generated/store_value.pb.h"
#include "dpkvs/core/store_record/generated/store_record.pb.h"

using NKVStore::NCore::NRecord::TStoreValue;
using NKVStore::NCore::NRecord::TStoreRecord;

namespace NKVStore::NCore::NRecord
{

TStoreRecord CreatePutRecord(const std::string &key, const TStoreValue &value);

TStoreRecord CreateRemoveRecord(const std::string &key);

} // namespace NKVStore::NCore::NRecord
