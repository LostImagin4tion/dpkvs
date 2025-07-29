#pragma once

#include <dpkvs/engine/store_engine.h>

#include <string>
#include <any>
#include <fstream>

namespace NKVStore::NAppendLog
{

enum class EAppendLogOperations {
    Put,
    Remove,
};

class TAppendOnlyLog
{
public:
    TAppendOnlyLog() = default;

    TAppendOnlyLog(TAppendOnlyLog&) = delete;
    TAppendOnlyLog& operator=(const TAppendOnlyLog&) = delete;

    TAppendOnlyLog(TAppendOnlyLog&&) noexcept;
    TAppendOnlyLog& operator=(TAppendOnlyLog&&) noexcept ;

    ~TAppendOnlyLog();

    void AppendToLog(
        const EAppendLogOperations& operation,
        const std::string& key,
        std::optional<const NEngine::TStorableValue> value);

    std::unique_ptr<NEngine::TStoreEngine> RecoverFromLog();

private:
    void OpenOStream();
    void OpenIStream();

    void WritePutLog(
        const std::string& key,
        const NEngine::TStorableValue& value);

    void WriteRemoveLog(const std::string& key);

    template <class T>
    T ReadBinary();

    EAppendLogOperations ReadCommand();
    std::string ReadKey();
    NEngine::TStorableValue ReadValue();

    std::string _fileName = "append-only-log.txt";

    std::ofstream _log_ostream;
    std::ifstream _log_istream;
};

} // namespace NKVStore::NAppendLog
