#include "store_engine.h"

#include <mutex>

using NKVStore::NCore::NEngine::EStoreEngineOperations;

namespace NKVStore::NCore::NEngine
{

THashMapStoreEngine::THashMapStoreEngine()
    : _logger(std::make_unique<TAppendOnlyLog>())
{
    _engine = _logger->RecoverFromLog();
}

THashMapStoreEngine::THashMapStoreEngine(const std::string& appendOnlyLogFileName)
    : _logger(std::make_unique<TAppendOnlyLog>(appendOnlyLogFileName))
{
    _engine = _logger->RecoverFromLog();
}

THashMapStoreEngine::THashMapStoreEngine(THashMapStoreEngine&& other) noexcept
    : _engine(std::move(other._engine))
    , _logger(std::move(other._logger))
{}

THashMapStoreEngine& THashMapStoreEngine::operator=(THashMapStoreEngine&& other) noexcept
{
    _engine = std::move(other._engine);
    _logger = std::move(other._logger);
    return *this;
}

void THashMapStoreEngine::Put(
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

TStoreRecordPtr THashMapStoreEngine::Get(const std::string& key) const
{
    return _engine->Get(key);
}

bool THashMapStoreEngine::Remove(const std::string& key)
{
    std::unique_lock lock(_appendOnlyLogMutex);

    if (_engine->Get(key)) {
        _logger->AppendRemoveOperation(key);
        return _engine->Remove(key);
    } else {
        return false;
    }
}

} // NKVStore::NCore::NEngine
