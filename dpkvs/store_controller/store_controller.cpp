#include "store_controller.h"

namespace NKVStore::NController
{

TStoreController::TStoreController()
    : _logger(std::make_unique<NAppendLog::TAppendOnlyLog>())
{
    _engine = _logger->RecoverFromLog();
}

TStoreController::TStoreController(TStoreController&& other) noexcept
    : _engine(std::move(other._engine))
    , _logger(std::move(other._logger))
{}

TStoreController& TStoreController::operator=(TStoreController&& other) noexcept
{
    _engine = std::move(other._engine);
    _logger = std::move(other._logger);
    return *this;
}

void TStoreController::Put(
    std::string& key,
    std::vector<uint8_t>& value)
{
    auto storableValue = NEngine::TStorableValue(std::move(value));

    _logger->AppendToLog(
        NAppendLog::EAppendLogOperations::Put,
        key,
        storableValue);

    _engine->Put(
        std::move(key),
        std::move(storableValue));
}

std::optional<NEngine::TStorableValue> TStoreController::Get(const std::string& key) const
{
    return _engine->Get(key);
}

bool TStoreController::Remove(const std::string& key)
{
    _logger->AppendToLog(
        NAppendLog::EAppendLogOperations::Remove,
        key,
        std::nullopt);

    return _engine->Remove(key);
}

} // NKVStore::NController
