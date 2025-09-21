#include "hash_map_store_engine.h"

#include <mutex>

namespace NKVStore::NCore::NEngine
{

THashMapStoreEngine::THashMapStoreEngine(
    const std::string& logFileName,
    std::shared_ptr<TConsoleLogger> logger)
    : _consoleLogger(std::move(logger))
    , _appendOnlyLog(std::make_unique<TAppendOnlyLog>(logFileName, _consoleLogger))
{
    _engine = _appendOnlyLog->RecoverFromLog();
}

THashMapStoreEngine::THashMapStoreEngine(THashMapStoreEngine&& other) noexcept
    : _consoleLogger(std::move(other._consoleLogger))
    , _appendOnlyLog(std::move(other._appendOnlyLog))
    , _engine(std::move(other._engine))
{}

THashMapStoreEngine& THashMapStoreEngine::operator=(THashMapStoreEngine&& other) noexcept
{
    _consoleLogger = std::move(other._consoleLogger);
    _appendOnlyLog = std::move(other._appendOnlyLog);
    _engine = std::move(other._engine);

    return *this;
}

void THashMapStoreEngine::Put(
    absl::string_view key,
    absl::string_view value)
{
    _consoleLogger->Debug("[THashMapStoreEngine] PUT - key {}, value_size {}", key, value.size());
    
    absl::MutexLock lock(&_mutex);

    try {
        TStoreValue storableValue;
        storableValue.set_data(std::string(value));

        _appendOnlyLog->AppendPutOperation(std::string(key), storableValue);
        _engine->Put(key, std::move(storableValue));
        
        _consoleLogger->Debug("[THashMapStoreEngine] PUT ok - key {}", key);
    } catch (const std::exception& e) {
        _consoleLogger->Error("[THashMapStoreEngine] PUT failed - key {}, error: {}", key, e.what());
        throw;
    }
}

TStoreValuePtr THashMapStoreEngine::Get(absl::string_view key) const
{
    _consoleLogger->Debug("[THashMapStoreEngine] GET - key {}", key);
    
    try {
        auto result = _engine->Get(key);

        if (result) {
            _consoleLogger->Debug("[THashMapStoreEngine] GET ok - key {}", key);
        } else {
            _consoleLogger->Debug("[THashMapStoreEngine] GET not_found - key {}", key);
        }
        
        return result;
    }
    catch (const std::exception& e) {
        _consoleLogger->Error("[THashMapStoreEngine] GET failed - key {}, error: {}", key, e.what());
        throw;
    }
}

bool THashMapStoreEngine::Remove(absl::string_view key)
{
    _consoleLogger->Debug("[THashMapStoreEngine] REMOVE - key: {}", key);
    
    absl::MutexLock lock(&_mutex);

    try {
        if (_engine->Get(key)) {
            _appendOnlyLog->AppendRemoveOperation(std::string(key));
            bool removed = _engine->Remove(key);
            
            _consoleLogger->Debug("[THashMapStoreEngine] REMOVE ok - key {}", key);
            return removed;
        } else {
            _consoleLogger->Debug("[THashMapStoreEngine] REMOVE not_found - key {}", key);
            return false;
        }
    }
    catch (const std::exception& e) {
        _consoleLogger->Error("[THashMapStoreEngine] REMOVE failed - key {}, error: {}", key, e.what());
        throw;
    }
}

} // NKVStore::NCore::NEngine
