#include <iostream>
#include "log_serializer.h"

namespace NKVStore::NAppendLog
{

TAppendLogSerializer::TAppendLogSerializer()
{
   OpenFileStream();
}

TAppendLogSerializer::TAppendLogSerializer(const std::string& fileName)
    : _fileName(fileName)
{
    OpenFileStream();
}

TAppendLogSerializer::TAppendLogSerializer(TAppendLogSerializer&& other) noexcept
    : _fileName(std::move(other._fileName))
    , _log_stream(std::move(other._log_stream))
{}

TAppendLogSerializer& TAppendLogSerializer::operator=(TAppendLogSerializer&& other) noexcept
{
    _fileName = std::move(other._fileName);
    _log_stream = std::move(other._log_stream);
    return *this;
}

TAppendLogSerializer::~TAppendLogSerializer()
{
    if (_log_stream.is_open()) {
        Flush();
        _log_stream.close();
    }
}

void TAppendLogSerializer::EnableReadMode()
{
    _log_stream.clear();
    _log_stream.seekg(0, std::ios::beg);
}

bool TAppendLogSerializer::ReadyToRead()
{
    return _log_stream.good() && !_log_stream.eof() && _log_stream.peek() != std::char_traits<char>::eof();
}

void TAppendLogSerializer::WritePutLog(
    const std::string& key,
    const NEngine::TStorableValue& value)
{
    EnableWriteMode();

    EAppendLogOperations operation = EAppendLogOperations::Put;
    _log_stream.write(reinterpret_cast<const char*>(&operation), sizeof(operation));

    uint32_t keySize = key.size();
    _log_stream.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));
    _log_stream.write(key.data(), keySize);

    uint32_t dataSize = value.data.size();
    _log_stream.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));

    _log_stream.write(value.data.data(), dataSize);
    _log_stream.write(reinterpret_cast<const char*>(&value.flags), sizeof(value.flags));

    uint8_t hasExpiry = value.expiry.has_value();
    _log_stream.write(reinterpret_cast<const char*>(&hasExpiry), sizeof(hasExpiry));

    if (hasExpiry) {
        int64_t time = value.expiry->time_since_epoch().count();
        _log_stream.write(reinterpret_cast<const char*>(&time), sizeof(time));
    }

    Flush();
}

void TAppendLogSerializer::WriteRemoveLog(const std::string& key)
{
    EnableWriteMode();

    EAppendLogOperations operation = EAppendLogOperations::Remove;
    _log_stream.write(reinterpret_cast<const char*>(&operation), sizeof(operation));

    uint32_t keySize = key.size();
    _log_stream.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));
    _log_stream.write(key.data(), keySize);

    Flush();
}

EAppendLogOperations TAppendLogSerializer::ReadCommand()
{
    EAppendLogOperations command;
    _log_stream.read(reinterpret_cast<char*>(&command), sizeof(command));

    if (!_log_stream.good()) {
        throw std::runtime_error("Failed to read operation");
    }

    return command;
}

std::string TAppendLogSerializer::ReadKey()
{
    auto keySize = ReadBinary<uint32_t>();
    std::string key(keySize, '\0');

    _log_stream.read(key.data(), keySize);

    if (!_log_stream.good()) {
        throw std::runtime_error("Failed to read key");
    }

    return key;
}

NEngine::TStorableValue TAppendLogSerializer::ReadValue()
{
    NEngine::TStorableValue value;

    auto dataSize = ReadBinary<uint32_t>();
    value.data.resize(dataSize);

    _log_stream.read(value.data.data(), dataSize);
    if (!_log_stream.good()) {
        throw std::runtime_error("Failed to read value data");
    }

    value.flags = ReadBinary<uint32_t>();

    uint8_t hasExpiry = ReadBinary<uint8_t>();

    if (hasExpiry) {
        auto timeValue = ReadBinary<int64_t>();
        value.expiry = std::chrono::system_clock::time_point{
            std::chrono::system_clock::duration{timeValue}
        };
    }

    return value;
}

void TAppendLogSerializer::OpenFileStream()
{
    _log_stream.open(
        _fileName,
        std::ios::binary | std::ios::in | std::ios::out);

    if (!_log_stream.is_open()) {
        _log_stream.clear();
        _log_stream.open(_fileName, std::ios::binary | std::ios::out);
        _log_stream.close();
        _log_stream.open(_fileName, std::ios::binary | std::ios::in | std::ios::out);
    }
}

void TAppendLogSerializer::EnableWriteMode()
{
    _log_stream.clear();
    _log_stream.seekp(0, std::ios::end);
}

void TAppendLogSerializer::Flush()
{
    _log_stream.flush();
}

} // namespace NKVStore::NAppendLog
