//
// Created by Egor Danilov on 29.07.2025.
//

#include "storable_value.h"

namespace NKVStore::Engine
{
    TStorableValue::TStorableValue(std::vector<uint8_t>&& bytes)
        : byteData(std::move(bytes))
    {}

} // namespace NKVStore::Engine
