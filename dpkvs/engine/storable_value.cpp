#include "storable_value.h"

namespace NKVStore::NEngine
{

TStorableValue::TStorableValue(std::vector<uint8_t>&& bytes)
    : binaryData(std::move(bytes))
{}

} // namespace NKVStore::NEngine
