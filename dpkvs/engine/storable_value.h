//
// Created by Egor Danilov on 29.07.2025.
//

#pragma once

#include <optional>
#include <vector>

namespace NKVStore::Engine
{
    struct TStorableValue
    {
        TStorableValue() = default;

        explicit TStorableValue( std::vector<uint8_t>&& bytes);

        TStorableValue(const TStorableValue&) = default;
        TStorableValue(TStorableValue&&) = default;

        TStorableValue& operator=(const TStorableValue&) = default;
        TStorableValue& operator=(TStorableValue&&) = default;

        std::vector<uint8_t> byteData;
        std::optional<std::chrono::system_clock::time_point> expiry;
        uint32_t flags = 0;
    };

} // NKVStore::Engine
