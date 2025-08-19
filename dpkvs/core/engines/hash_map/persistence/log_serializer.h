#pragma once

#include <dpkvs/core/engines/operations.h>
#include <dpkvs/core/store_value/generated/store_value.pb.h>

#include <string>
#include <fstream>

using NKVStore::NCore::NRecord::TStoreValue;
using NKVStore::NCore::NEngine::EStoreEngineOperations;

namespace NKVStore::NCore::NEngine::NPersistence
{

    class TAppendLogSerializer
    {
    public:
        TAppendLogSerializer();

        explicit TAppendLogSerializer(std::string fileName);

        TAppendLogSerializer(TAppendLogSerializer&) = delete;
        TAppendLogSerializer& operator=(const TAppendLogSerializer&) = delete;

        TAppendLogSerializer(TAppendLogSerializer&&) noexcept;
        TAppendLogSerializer& operator=(TAppendLogSerializer&&) noexcept;

        ~TAppendLogSerializer();

        void EnableReadMode();
        bool ReadyToRead();

        void WritePutLog(
            const std::string& key,
            const TStoreValue& value);

        void WriteRemoveLog(const std::string& key);

        EStoreEngineOperations ReadCommand();
        std::string ReadKey();
        TStoreValue ReadValue();

    private:
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

} // namespace NKVStore::NCore::NEngine::NPersistence
