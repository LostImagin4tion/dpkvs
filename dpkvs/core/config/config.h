#pragma once

#include <yaml-cpp/yaml.h>

namespace NKVStore::NCore::NConfig {

class TConfig {
public:
    explicit TConfig(YAML::Node root, const std::string& envPrefix = _defaultEnvPrefix);

    bool FeatureEnabled(
        const std::string& key,
        bool defValue = false) const;

    const std::string GetString(
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

    static const TConfig FromFile(
        const std::string& yamlPath,
        const std::string& envPrefix = _defaultEnvPrefix)
    {
        return TConfig(YAML::LoadFile(yamlPath), envPrefix);
    }

private:
    static std::vector<const std::string> Split(const std::string& key, char delim) {
        std::vector<const std::string> out;
        std::stringstream stream(key);

        std::string item;

        while (std::getline(stream, item, delim)) {
            out.push_back(item);
        }
        return out;
    }

    static std::optional<const std::string> GetEnv(const std::string& name) {
        if (const char* v = std::getenv(name.c_str())) {
            return std::string(v);
        }
        return std::nullopt;
    }

    static bool ParseBool(const std::string& s, bool fallback) {
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

    static int64_t ParseInt(const std::string &s, int64_t fallback) {
        try {
            return std::stoll(s);
        } catch (...) {
            return fallback;
        }
    }

    static double ParseDouble(const std::string& s, double fallback) {
        try {
            return std::stod(s);
        } catch (...) {
            return fallback;
        }
    }

    static std::string ToEnvUpper(const std::string& key) {
        std::string out;
        out.reserve(key.size() + 8);

        for (char c: key) {
            if (c == '.' || c == '-' || c == ' ') {
                out.push_back('_');
            } else {
                out.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
            }
        }
        return out;
    }

    std::string ToEnvName(const std::string& key) const {
        return _envPrefix + ToEnvUpper(key);
    }

    std::optional<YAML::Node> GetYaml(const std::string& key) const {
        const auto keyParts = Split(key, '.');
        YAML::Node currentNode = _root;

        for (const auto& part: keyParts) {
            if (!currentNode || !currentNode.IsMap()) {
                return std::nullopt;
            }
            currentNode = currentNode[part];
        }

        if (currentNode) {
            return currentNode;
        }
        return std::nullopt;
    }

    static constexpr std::string _defaultEnvPrefix = "DPKVS_";

    YAML::Node _root;
    std::string _envPrefix;

};

}