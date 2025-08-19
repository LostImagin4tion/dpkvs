#include "record_serializer.h"

#include <iostream>
#include <utility>

namespace NKVStore::NCore::NRecord
{

TStoreRecordSerializer::TStoreRecordSerializer()
{
   OpenFileStream();
}

TStoreRecordSerializer::TStoreRecordSerializer(std::string  fileName)
    : _fileName(std::move(fileName))
{
    OpenFileStream();
}

TStoreRecordSerializer::TStoreRecordSerializer(TStoreRecordSerializer&& other) noexcept
    : _fileName(std::move(other._fileName))
    , _log_stream(std::move(other._log_stream))
{}

TStoreRecordSerializer& TStoreRecordSerializer::operator=(TStoreRecordSerializer&& other) noexcept
{
    _fileName = std::move(other._fileName);
    _log_stream = std::move(other._log_stream);
    return *this;
}

TStoreRecordSerializer::~TStoreRecordSerializer()
{
    if (_log_stream.is_open()) {
        Flush();
        _log_stream.close();
    }
}

void TStoreRecordSerializer::EnableReadMode()
{
    _log_stream.clear();
    _log_stream.seekg(0, std::ios::beg);
}

bool TStoreRecordSerializer::ReadyToRead()
{
    return _log_stream.good() && !_log_stream.eof() && _log_stream.peek() != std::char_traits<char>::eof();
}

void TStoreRecordSerializer::WriteRecord(const TStoreRecord& record)
{
    if (record.has_put_operation()) {
        WritePutLog(record);
    } else if (record.has_remove_operation()) {
        WriteRemoveLog(record);
    }
}

EStoreEngineOperations TStoreRecordSerializer::ReadCommand()
{
    EStoreEngineOperations command;
    _log_stream.read(reinterpret_cast<char*>(&command), sizeof(command));

    if (!_log_stream.good()) {
        throw std::runtime_error("Failed to read operation");
    }

    return command;
}

std::string TStoreRecordSerializer::ReadKey()
{
    auto keySize = ReadBinary<uint32_t>();
    std::string key(keySize, '\0');

    _log_stream.read(key.data(), keySize);

    if (!_log_stream.good()) {
        throw std::runtime_error("Failed to read key");
    }

    return key;
}

TStoreValue TStoreRecordSerializer::ReadValue()
{
    TStoreValue value;

    auto dataSize = ReadBinary<uint32_t>();
    auto* dataStr = value.mutable_data();
    dataStr->resize(dataSize);

    _log_stream.read(dataStr->data(), dataSize);
    if (!_log_stream.good()) {
        throw std::runtime_error("Failed to read value data");
    }

    value.set_flags(ReadBinary<uint32_t>());

    uint8_t hasExpiry = ReadBinary<uint8_t>();

    if (hasExpiry) {
        auto timeValue = ReadBinary<int64_t>();
        value.set_expiry_millis(timeValue);
    }

    return value;
}

void TStoreRecordSerializer::WritePutLog(const TStoreRecord& record)
{
    EnableWriteMode();

    auto key = record.put_operation().key();
    auto value = record.put_operation().value();

    auto operation = EStoreEngineOperations::Put;
    _log_stream.write(reinterpret_cast<const char*>(&operation), sizeof(operation));

    uint32_t keySize = key.size();
    _log_stream.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));
    _log_stream.write(key.data(), keySize);

    uint32_t dataSize = value.data().size();
    _log_stream.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));

    _log_stream.write(value.data().data(), dataSize);

    auto flags = value.flags();
    _log_stream.write(reinterpret_cast<const char*>(&flags), sizeof(flags));

    uint8_t hasExpiry = value.has_expiry_millis();
    _log_stream.write(reinterpret_cast<const char*>(&hasExpiry), sizeof(hasExpiry));

    if (hasExpiry) {
        int64_t time = value.expiry_millis();
        _log_stream.write(reinterpret_cast<const char*>(&time), sizeof(time));
    }

    Flush();
}

void TStoreRecordSerializer::WriteRemoveLog(const TStoreRecord& record)
{
    EnableWriteMode();

    auto key = record.remove_operation().key();

    auto operation = EStoreEngineOperations::Remove;
    _log_stream.write(reinterpret_cast<const char*>(&operation), sizeof(operation));

    uint32_t keySize = key.size();
    _log_stream.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));
    _log_stream.write(key.data(), keySize);

    Flush();
}

void TStoreRecordSerializer::OpenFileStream()
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

void TStoreRecordSerializer::EnableWriteMode()
{
    _log_stream.clear();
    _log_stream.seekp(0, std::ios::end);
}

void TStoreRecordSerializer::Flush()
{
    _log_stream.flush();
}

} // namespace NKVStore::NCore::NRecord
