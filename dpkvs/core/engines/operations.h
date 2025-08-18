#pragma once

#include <cstdint>

namespace NKVStore::NCore::NEngine
{

enum class EStoreEngineOperations : uint8_t {
    Put = 1,
    Remove = 2,
};

} // NKVStore::NCore::NEngine
