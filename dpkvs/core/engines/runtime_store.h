#pragma once

#include <dpkvs/core/store_value/generated/store_value.pb.h>

#include <absl/strings/string_view.h>

using NKVStore::NCore::NRecord::TStoreValue;

using TStoreValuePtr = std::shared_ptr<const TStoreValue>;

namespace NKVStore::NCore::NEngine::NRuntime
{

struct IRuntimeStore {
    virtual ~IRuntimeStore() = default;

    virtual void Put(absl::string_view key, TStoreValue value) = 0;

    [[nodiscard]] virtual TStoreValuePtr Get(absl::string_view key) const = 0;

    [[nodiscard]] virtual bool Remove(absl::string_view key) = 0;

    [[nodiscard]] virtual size_t Size() const = 0;
};

} // namespace NKVStore::NCore::NEngine::NRuntime
