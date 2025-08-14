#include "store_controller.h"

#include <mutex>

using NKVStore::NCore::NEngine::EStoreEngineOperations;

namespace NKVStore::NCore::NEngine::NController
{

THashMapStoreController::THashMapStoreController()
    : _logger(std::make_unique<TAppendOnlyLog>())
{
    _engine = _logger->RecoverFromLog();
}

THashMapStoreController::THashMapStoreController(const std::string& appendOnlyLogFileName)
    : _logger(std::make_unique<TAppendOnlyLog>(appendOnlyLogFileName))
{
    _engine = _logger->RecoverFromLog();
}

THashMapStoreController::THashMapStoreController(THashMapStoreController&& other) noexcept
    : _engine(std::move(other._engine))
    , _logger(std::move(other._logger))
{}

THashMapStoreController& THashMapStoreController::operator=(THashMapStoreController&& other) noexcept
{
    _engine = std::move(other._engine);
    _logger = std::move(other._logger);
    return *this;
}

void THashMapStoreController::Put(
    std::string key,
    std::string value)
{
    std::lock_guard<std::mutex> lock(_appendOnlyLogMutex);

    TStoreRecord storableValue(std::move(value));
    _logger->AppendPutOperation(key,storableValue);

    _engine->Put(
        std::move(key),
        std::move(storableValue));
}

TStoreRecordPtr THashMapStoreController::Get(const std::string& key) const
{
    return _engine->Get(key);
}

bool THashMapStoreController::Remove(const std::string& key)
{
    std::unique_lock lock(_appendOnlyLogMutex);

    if (_engine->Get(key)) {
        _logger->AppendRemoveOperation(key);
        return _engine->Remove(key);
    } else {
        return false;
    }
}

} // NKVStore::NCore::NEngine::NController
