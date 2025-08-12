#include "store_controller.h"

using NKVStore::NAppendLog::TAppendOnlyLog;
using NKVStore::NAppendLog::EAppendLogOperations;

namespace NKVStore::NController
{

TStoreController::TStoreController()
    : _logger(std::make_unique<TAppendOnlyLog>())
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
    std::string key,
    std::string value)
{
    TStorableValue storableValue(std::move(value));

    _appendOnlyLogMutex.lock();
    _logger->AppendPutOperation(key,storableValue);
    _appendOnlyLogMutex.unlock();

    _engine->Put(
        std::move(key),
        std::move(storableValue));
}

TStorableValuePtr TStoreController::Get(const std::string& key) const
{
    return _engine->Get(key);
}

bool TStoreController::Remove(const std::string& key)
{
    if (_engine->Get(key)) {
        _appendOnlyLogMutex.lock();
        _logger->AppendRemoveOperation(key);
        _appendOnlyLogMutex.unlock();

        return _engine->Remove(key);
    } else {
        return false;
    }
}

} // NKVStore::NController
