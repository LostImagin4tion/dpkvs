#pragma once

namespace NKVStore::NAppendLog
{

enum class EAppendLogOperations : uint8_t {
    Put = 1,
    Remove = 2,
};

}