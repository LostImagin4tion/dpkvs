#pragma once

#include <string>
#include <fstream>

#include <dpkvs/engine/storable_value.h>
#include <dpkvs/append_only_log/operations.h>

namespace NKVStore::NAppendLog
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
            const NEngine::TStorableValue& value);

        void WriteRemoveLog(const std::string& key);

        EAppendLogOperations ReadCommand();
        std::string ReadKey();
        NEngine::TStorableValue ReadValue();

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

} // namespace NKVStore::NAppendLog
