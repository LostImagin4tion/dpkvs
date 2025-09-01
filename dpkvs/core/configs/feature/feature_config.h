#pragma once

#include <yaml-cpp/yaml.h>
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

class TFeatureConfig
{
public:

    template<class T>
    const T Get(const std::string& key, const T& defaultValue) const
    {
        if (auto node = GetYaml(key)) {
            return node->as<T>(defaultValue);
        }
        return defaultValue;
    }

    static TFeatureConfig FromFile(const std::string& yamlPath)
    {
        return TFeatureConfig(YAML::LoadFile(yamlPath));
    }

private:
    static std::vector<std::string> Split(const std::string& key, char delim)
    {
        std::vector<std::string> out;
        std::stringstream stream(key);

        std::string item;

        while (std::getline(stream, item, delim)) {
            out.push_back(item);
        }
        return out;
    }

    explicit TFeatureConfig(YAML::Node&& root);

    std::optional<YAML::Node> GetYaml(const std::string& key) const
    {
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

    YAML::Node _root;
};

} // namespace NKVStore::NCore::NConfig
