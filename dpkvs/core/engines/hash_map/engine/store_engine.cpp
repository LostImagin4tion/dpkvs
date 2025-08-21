#include "store_engine.h"

#include <mutex>

namespace NKVStore::NCore::NEngine
{

THashMapStoreEngine::THashMapStoreEngine()
    : _logger(std::make_unique<TAppendOnlyLog>())
{
    _engine = _logger->RecoverFromLog();
}

THashMapStoreEngine::THashMapStoreEngine(const std::string& logFileName)
    : _logger(std::make_unique<TAppendOnlyLog>(logFileName))
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
    absl::string_view key,
    absl::string_view value)
{
    absl::MutexLock lock(&_mutex);

    TStoreValue storableValue;
    storableValue.set_data(std::string(value));

    _logger->AppendPutOperation(std::string(key), storableValue);

    _engine->Put(key,std::move(storableValue));
}

TStoreValuePtr THashMapStoreEngine::Get(absl::string_view key) const
{
    return _engine->Get(key);
}

bool THashMapStoreEngine::Remove(absl::string_view key)
{
    absl::MutexLock lock(&_mutex);

    if (_engine->Get(key)) {
        _logger->AppendRemoveOperation(std::string(key));
        return _engine->Remove(key);
    } else {
        return false;
    }
}

} // NKVStore::NCore::NEngine
