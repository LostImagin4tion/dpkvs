#pragma once

#include <dpkvs/core/store_record/serializer/operations.h>
#include <dpkvs/core/store_record/generated/store_record.pb.h>

#include <string>
#include <fstream>

using NKVStore::NCore::NRecord::TStoreRecord;
using NKVStore::NCore::NRecord::EStoreEngineOperations;

namespace NKVStore::NCore::NRecord
{

    class TStoreRecordSerializer
    {
    public:
        TStoreRecordSerializer();

        explicit TStoreRecordSerializer(std::string fileName);

        TStoreRecordSerializer(TStoreRecordSerializer&) = delete;
        TStoreRecordSerializer& operator=(const TStoreRecordSerializer&) = delete;

        TStoreRecordSerializer(TStoreRecordSerializer&&) noexcept;
        TStoreRecordSerializer& operator=(TStoreRecordSerializer&&) noexcept;

        ~TStoreRecordSerializer();

        void EnableReadMode();
        bool ReadyToRead();

        void WriteRecord(const TStoreRecord& record);

        EStoreEngineOperations ReadCommand();
        std::string ReadKey();
        TStoreValue ReadValue();

    private:
        void WritePutLog(const TStoreRecord& record);
        void WriteRemoveLog(const TStoreRecord& record);

        void OpenFileStream();
        void EnableWriteMode();
        void Flush();

        template <class T>
        T ReadBinary()
        {
            T value;
            _log_stream.read(reinterpret_cast<char*>(&value), sizeof(value));

            if (!_log_stream.good()) {
                throw std::runtime_error("Failed to read binary data");
            }

            return value;
        }

        std::fstream _log_stream;

        std::string _fileName = "append-only-log.txt";
    };

} // namespace NKVStore::NCore::NRecord
