#include "config.h"

namespace NKVStore::NCore::NConfig
{

TConfig::TConfig(YAML::Node root, const std::string& envPrefix)
    : _root(std::move(root))
    , _envPrefix(std::move(envPrefix))
{}

bool TConfig::FeatureEnabled(
    const std::string& featureName,
    bool defValue) const
{
    const std::string envName = "FEATURE_" + ToEnvUpper(featureName);

    if (const auto& env = GetEnv(envName)) {
        return ParseBool(*env, defValue);
    }

    const YAML::Node features = _root["features"];
    if (features && features.IsMap()) {
        const YAML::Node node = features[featureName];
        if (node) {
            return node.as<bool>(defValue);
        }
    }
    return defValue;
}

const std::string TConfig::GetString(
    const std::string& key,
    const std::string& defValue) const
{
    if (const auto& env = GetEnv(ToEnvName(key))) {
        return *env;
    }
    if (const auto& node = GetYaml(key)) {
        return node->as<std::string>(defValue);
    }
    return defValue;
}

bool TConfig::GetBool(
    const std::string &key,
    bool defaultValue) const
{
    if (auto env = GetEnv(ToEnvName(key))) {
        return ParseBool(*env, defaultValue);
    }
    if (auto node = GetYaml(key)) {
        return node->as<bool>(defaultValue);
    }
    return defaultValue;
}

int64_t TConfig::GetInt(
    const std::string &key,
    int64_t defaultValue) const
{
    if (auto env = GetEnv(ToEnvName(key))) {
        return ParseInt(*env, defaultValue);
    }
    if (auto node = GetYaml(key)) {
        return node->as<int64_t>(defaultValue);
    }
    return defaultValue;
}

double TConfig::GetDouble(
    const std::string &key,
    double defaultValue) const
{
    if (auto env = GetEnv(ToEnvName(key))) {
        return ParseDouble(*env, defaultValue);
    }
    if (auto node = GetYaml(key)) {
        return node->as<double>(defaultValue);
    }
    return defaultValue;
}

}
