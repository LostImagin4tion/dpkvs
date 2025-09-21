#include "build_config.h"

namespace NKVStore::NCore::NConfig
{

std::string TBuildConfig::GetString(
    const std::string& key,
    const std::string& defValue) const
{
    if (const auto& env = GetEnv(ToEnvName(key))) {
        return *env;
    }
    return defValue;
}

bool TBuildConfig::GetBool(
    const std::string &key,
    bool defaultValue) const
{
    if (auto env = GetEnv(ToEnvName(key))) {
        return ParseBool(*env, defaultValue);
    }
    return defaultValue;
}

int64_t TBuildConfig::GetInt(
    const std::string &key,
    int64_t defaultValue) const
{
    if (auto env = GetEnv(ToEnvName(key))) {
        return ParseInt(*env, defaultValue);
    }
    return defaultValue;
}

double TBuildConfig::GetDouble(
    const std::string &key,
    double defaultValue) const
{
    if (auto env = GetEnv(ToEnvName(key))) {
        return ParseDouble(*env, defaultValue);
    }
    return defaultValue;
}

} // namespace NKVStore::NCore::NConfig
