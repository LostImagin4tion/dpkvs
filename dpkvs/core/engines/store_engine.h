#pragma once

#include <dpkvs/core/engines/runtime_store.h>

#include <absl/strings/string_view.h>

namespace NKVStore::NCore::NEngine {

struct IStoreEngine {
    virtual ~IStoreEngine() = default;

    virtual void Put(absl::string_view key, absl::string_view value) = 0;

    [[nodiscard]] virtual TStoreValuePtr Get(absl::string_view key) const = 0;

    [[nodiscard]] virtual bool Remove(absl::string_view key) = 0;
};

} // namespace NKVStore::NCore::NEngine
