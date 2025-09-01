#include "record_serializer.h"

#include <absl/crc/crc32c.h>

#include <iostream>
#include <utility>
#include <system_error>
#include <filesystem>
#include <fcntl.h>
#include <unistd.h>

namespace NKVStore::NCore::NRecord
{

TStoreRecordSerializer::TStoreRecordSerializer(std::string fileName)
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
        try {
            Flush();
        } catch (...) {}

        _log_stream.close();
    }
    if (_fileDescriptor >= 0) {
        close(_fileDescriptor);
        _fileDescriptor = -1;
    }
}

void TStoreRecordSerializer::EnableReadMode()
{
    _log_stream.clear();
    _log_stream.seekg(0, std::ios::beg);
}

void TStoreRecordSerializer::WriteRecord(const TStoreRecord& record)
{
    std::string serializedRecord;
    serializedRecord.reserve(record.ByteSizeLong());

    if (!record.SerializeToString(&serializedRecord)) {
        throw std::runtime_error("Failed to serialize protobuf record to string");
    }

    const auto recordSize = static_cast<uint32_t>(serializedRecord.size());
    const auto crc32 = static_cast<uint32_t>(absl::ComputeCrc32c(serializedRecord));

    EnableWriteMode();

    WriteU32LE(recordSize);
    WriteU32LE(crc32);
    _log_stream.write(serializedRecord.data(), serializedRecord.size());

    if (!_log_stream.good()) {
        throw std::runtime_error("WriteRecord failed");
    }

    Flush();
}

bool TStoreRecordSerializer::ReadRecord(TStoreRecord& outRecord)
{
    if (_log_stream.peek() == std::char_traits<char>::eof()) {
        return false;
    }

    uint32_t recordSize = 0u;
    try {
        recordSize = ReadU32LE();
    } catch (...) {
        if (_log_stream.eof()) {
            return false;
        }
        throw;
    }

    uint32_t storedCrc32 = ReadU32LE();

    std::string serializedRecord(recordSize, '\0');
    _log_stream.read(serializedRecord.data(), static_cast<std::streamsize>(recordSize));
    if (!_log_stream.good()) {
        throw std::runtime_error("Failed to read record payload");
    }

    const uint32_t actualCrc32 = static_cast<uint32_t>(absl::ComputeCrc32c(serializedRecord));
    if (actualCrc32 != storedCrc32) {
        throw std::runtime_error("CRC32C mismatch");
    }

    if (!outRecord.ParseFromArray(serializedRecord.data(), static_cast<int>(serializedRecord.size()))) {
        throw std::runtime_error("Failed to parse protobuf record");
    }
    return true;
}


void TStoreRecordSerializer::OpenFileStream()
{
    _log_stream.open(
        _fileName,
        std::ios::binary | std::ios::in | std::ios::out);

    if (!_log_stream.is_open()) {
        std::filesystem::path filePath(_fileName);
        std::filesystem::path parentDir = filePath.parent_path();

        if (!parentDir.empty()) {
            std::filesystem::create_directories(parentDir);
        }

        _log_stream.clear();
        _log_stream.open(_fileName, std::ios::binary | std::ios::out);
        _log_stream.close();
        _log_stream.open(_fileName, std::ios::binary | std::ios::in | std::ios::out);
    }

    // Open OS fd for fsync
    _fileDescriptor = open(_fileName.c_str(), O_RDWR | O_CLOEXEC);
    if (_fileDescriptor < 0) {
        throw std::system_error(errno, std::generic_category(), "open() for fsync failed");
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
    if (!_log_stream.good()) {
        throw std::runtime_error("flush failed");
    }
    Fsync();
}

void TStoreRecordSerializer::WriteU32LE(uint32_t value)
{
    char b[4] = {
        static_cast<char>(value & 0xFF),
        static_cast<char>((value >> 8) & 0xFF),
        static_cast<char>((value >> 16) & 0xFF),
        static_cast<char>((value >> 24) & 0xFF)
    };
    _log_stream.write(b, 4);
}

uint32_t TStoreRecordSerializer::ReadU32LE()
{
    char value[4];
    _log_stream.read(value, 4);
    if (!_log_stream.good()) {
        throw std::runtime_error("Failed to read u32");
    }
    return (static_cast<uint32_t>(static_cast<unsigned char>(value[0]))      ) |
           (static_cast<uint32_t>(static_cast<unsigned char>(value[1])) << 8 ) |
           (static_cast<uint32_t>(static_cast<unsigned char>(value[2])) << 16) |
           (static_cast<uint32_t>(static_cast<unsigned char>(value[3])) << 24);
}

void TStoreRecordSerializer::Fsync() const {
#ifdef __APPLE__
    if (_fileDescriptor >= 0 && fcntl(_fileDescriptor, F_FULLFSYNC) != 0 && fsync(_fileDescriptor) != 0) {
        throw std::system_error(errno, std::generic_category(), "fsync failed");
    }
#else
    if (_fileDescriptor >= 0 && fsync(_fileDescriptor) != 0) {
        throw std::system_error(errno, std::generic_category(), "fsync failed");
    }
#endif
}

} // namespace NKVStore::NCore::NRecord
