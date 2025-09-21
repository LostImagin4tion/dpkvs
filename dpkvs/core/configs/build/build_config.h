#pragma once

#include <string>
#include <vector>
#include <optional>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <cstdint>
#include <cstdlib>

namespace NKVStore::NCore::NConfig
{

class TBuildConfig
{
public:
    std::string GetString(
        const std::string& key,
        const std::string& defValue = "") const;

    bool GetBool(
        const std::string& key,
        bool defValue = false) const;

    int64_t GetInt(
        const std::string& key,
        int64_t defValue = 0) const;

    double GetDouble(
        const std::string& key,
        double defValue = 0.0) const;

private:
    static std::optional<const std::string> GetEnv(const std::string& name)
    {
        if (const char* v = std::getenv(name.c_str())) {
            return std::string(v);
        }
        return std::nullopt;
    }

    [[nodiscard]] static std::string ToEnvName(const std::string& key)
    {
        std::string out;
        out.reserve(key.size());

        for (char c: key) {
            out.push_back(std::toupper(c));
        }
        return out;
    }

    static bool ParseBool(const std::string& s, bool fallback)
    {
        std::string lowerString;
        lowerString.reserve(s.size());

        std::transform(
            s.begin(),
            s.end(),
            std::back_inserter(lowerString),
            [](unsigned char c) {
                return std::tolower(c);
            });

        if (lowerString == "true") {
            return true;
        }
        if (lowerString == "false") {
            return false;
        }
        return fallback;
    }

    static int64_t ParseInt(const std::string &s, int64_t fallback)
    {
        try {
            return std::stoll(s);
        } catch (...) {
            return fallback;
        }
    }

    static double ParseDouble(const std::string& s, double fallback)
    {
        try {
            return std::stod(s);
        } catch (...) {
            return fallback;
        }
    }
};

} // namespace NKVStore::NCore::NConfig
