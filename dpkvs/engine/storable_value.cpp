#include "storable_value.h"

namespace NKVStore::NEngine
{

TStorableValue::TStorableValue(std::string bytes)
    : data(std::move(bytes))
{
    expiry = std::chrono::system_clock::now() + std::chrono::hours(24);
}

} // namespace NKVStore::NEngine
