#pragma once

#include <dpkvs/core/store_record/generated/store_record.pb.h>

#include <string>
#include <fstream>

using NKVStore::NCore::NRecord::TStoreRecord;

namespace NKVStore::NCore::NRecord
{

    class TStoreRecordSerializer
    {
    public:
        TStoreRecordSerializer() = delete;

        explicit TStoreRecordSerializer(std::string fileName);

        TStoreRecordSerializer(TStoreRecordSerializer&) = delete;
        TStoreRecordSerializer& operator=(const TStoreRecordSerializer&) = delete;

        TStoreRecordSerializer(TStoreRecordSerializer&&) noexcept;
        TStoreRecordSerializer& operator=(TStoreRecordSerializer&&) noexcept;

        ~TStoreRecordSerializer();

        void EnableReadMode();

        void WriteRecord(const TStoreRecord& record);
        bool ReadRecord(TStoreRecord& outRecord);

    private:
        void OpenFileStream();
        void EnableWriteMode();
        void Flush();

        void WriteU32LE(uint32_t value);
        uint32_t ReadU32LE();

        void Fsync() const;

        std::fstream _log_stream;

        std::string _fileName;
        int _fileDescriptor = -1; // OS file descriptor used for fsync
    };

} // namespace NKVStore::NCore::NRecord
