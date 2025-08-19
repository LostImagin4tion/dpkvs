#pragma once

#include <cstdint>

namespace NKVStore::NCore::NRecord
{

enum class EStoreEngineOperations : uint8_t {
    Put = 1,
    Remove = 2,
};

} // NKVStore::NCore::NRecord
