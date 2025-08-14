#include "store_record.h"

namespace NKVStore::NCore::NRecord
{

TStoreRecord::TStoreRecord(std::string bytes)
    : data(std::move(bytes))
{
    expiry = std::chrono::system_clock::now() + std::chrono::hours(24);
}

} // namespace NKVStore::NCore::NRecord
