#pragma once

#include <dpkvs/engine/store_engine.h>
#include <dpkvs/append_only_log/append_only_log.h>

namespace NKVStore::NController
{

class TStoreController
{
public:
    TStoreController();

    TStoreController(const TStoreController&) = delete;
    TStoreController& operator=(const TStoreController&) = delete;

    TStoreController(TStoreController&&) noexcept;
    TStoreController& operator=(TStoreController&&) noexcept;

    ~TStoreController() = default;

    void Put(std::string& key, std::vector<uint8_t>& value);

    [[nodiscard]] std::optional<NEngine::TStorableValue> Get(const std::string& key) const;

    [[nodiscard]] bool Remove(const std::string& key);

private:
    std::unique_ptr<NEngine::TStoreEngine> _engine;
    std::unique_ptr<NAppendLog::TAppendOnlyLog> _logger;
};

} // NKVStore::NController
