#include "storable_value.h"

namespace NKVStore::NEngine
{

TStorableValue::TStorableValue(std::vector<uint8_t>&& bytes)
    : binaryData(std::move(bytes))
{
    expiry = std::chrono::system_clock::now() + std::chrono::hours(24);
}

} // namespace NKVStore::NEngine
