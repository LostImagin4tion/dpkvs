#include "append_only_log.h"

#include <iostream>

namespace NKVStore::NAppendLog
{

TAppendOnlyLog::TAppendOnlyLog(TAppendOnlyLog && other) noexcept
    : _fileName(std::move(other._fileName))
    , _log_ostream(std::move(other._log_ostream))
    , _log_istream(std::move(other._log_istream))
{}

TAppendOnlyLog& TAppendOnlyLog::operator=(TAppendOnlyLog && other) noexcept
{
    _fileName = std::move(other._fileName);
    _log_ostream = std::move(other._log_ostream);
    _log_istream = std::move(other._log_istream);
    return *this;
}

TAppendOnlyLog::~TAppendOnlyLog()
{
    if (_log_ostream.is_open()) {
        _log_ostream.close();
    }
    if (_log_istream.is_open()) {
        _log_istream.close();
    }
}

void TAppendOnlyLog::AppendToLog(
    const EAppendLogOperations& operation,
    const std::string& key,
    std::optional<const NEngine::TStorableValue> value)
{
    OpenOStream();

    switch (operation) {
        case EAppendLogOperations::Put:
            if (!value.has_value()) {
                throw std::invalid_argument("If you use put operation, value must contain not null");
            }
            WritePutLog(key, value.value());
            break;

        case EAppendLogOperations::Remove:
            WriteRemoveLog(key);
            break;
    }

        _log_ostream << "\n";
        _log_ostream.flush();
    }

std::unique_ptr<NEngine::TStoreEngine> TAppendOnlyLog::RecoverFromLog()
{
    OpenIStream();

    auto recoveredStore = std::unordered_map<std::string, NEngine::TStorableValue>();

    try {
        while (_log_istream.good() && !_log_istream.eof()) {
            if (_log_istream.peek() == EOF) {
                break;
            }

            EAppendLogOperations command = ReadCommand();
            switch (command) {
                case EAppendLogOperations::Put: {
                    std::string key = ReadKey();
                    NEngine::TStorableValue value = ReadValue();
                    recoveredStore[std::move(key)] = std::move(value);
                    break;
                }

                case EAppendLogOperations::Remove: {
                    std::string key = ReadKey();
                    recoveredStore.erase(key);
                    break;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error reading log: " << e.what() << std::endl;
    }

    return std::make_unique<NEngine::TStoreEngine>(std::move(recoveredStore));
}

void TAppendOnlyLog::OpenOStream()
{
    if (!_log_ostream.is_open()) {
        _log_ostream.open(_fileName, std::ios::binary);
    }
}

void TAppendOnlyLog::OpenIStream()
{
    if (!_log_istream.is_open()) {
        _log_istream.open(_fileName, std::ios::binary);
    }
}

void TAppendOnlyLog::WritePutLog(
    const std::string& key,
    const NEngine::TStorableValue& value)
{
    _log_ostream << "PUT ";

    uint32_t keySize = key.size();
    _log_ostream.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));
    _log_ostream.write(key.data(), keySize);

    uint32_t dataSize = value.binaryData.size();
    _log_ostream.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));

    _log_ostream.write(reinterpret_cast<const char*>(value.binaryData.data()), dataSize);
    _log_ostream.write(reinterpret_cast<const char*>(&value.flags), sizeof(value.flags));

    bool hasExpiry = value.expiry.has_value();
    _log_ostream.write(reinterpret_cast<const char*>(&hasExpiry), sizeof(hasExpiry));

    if (hasExpiry) {
        auto time = value.expiry->time_since_epoch().count();
        _log_ostream.write(reinterpret_cast<const char*>(&time), sizeof(time));
    }
}

void TAppendOnlyLog::WriteRemoveLog(const std::string& key)
{
    _log_ostream << "REMOVE ";

    uint32_t keySize = key.size();
    _log_ostream.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));
    _log_ostream.write(key.data(), keySize);
}

EAppendLogOperations TAppendOnlyLog::ReadCommand()
{
    std::string operation;
    _log_istream >> operation;

    if (!_log_istream.good()) {
        throw std::runtime_error("Failed to read operation");
    }

    if (operation == "PUT") {
        return EAppendLogOperations::Put;
    } else if (operation == "REMOVE") {
        return EAppendLogOperations::Remove;
    } else {
        throw std::runtime_error(std::format("Unknown operation: {}", operation));
    }
}

template <class T>
T TAppendOnlyLog::ReadBinary()
{
    T value;
    _log_istream.read(reinterpret_cast<char*>(&value), sizeof(value));

    if (!_log_istream.good()) {
        throw std::runtime_error("Failed to read binary data");
    }

    return value;
}

std::string TAppendOnlyLog::ReadKey()
{
    auto keySize = ReadBinary<uint32_t>();
    std::string key(keySize, '\0');

    _log_istream.read(key.data(), keySize);
    if (!_log_istream.good()) {
        throw std::runtime_error("Failed to read key");
    }

    return key;
}

NEngine::TStorableValue TAppendOnlyLog::ReadValue()
{
    NEngine::TStorableValue value;

    auto dataSize = ReadBinary<uint32_t>();
    value.binaryData.resize(dataSize);

    _log_istream.read(reinterpret_cast<char*>(value.binaryData.data()), dataSize);
    if (!_log_istream.good()) {
        throw std::runtime_error("Failed to read value data");
    }

    value.flags = ReadBinary<uint32_t>();

    bool hasExpiry = ReadBinary<bool>();

    if (hasExpiry) {
        auto timeValue = ReadBinary<int64_t>();
        value.expiry = std::chrono::system_clock::time_point{
            std::chrono::system_clock::duration{timeValue}
        };
    }

    return value;
}

} // namespace NKVStore::NAppendLog
