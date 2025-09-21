#include "feature_config.h"

namespace NKVStore::NCore::NConfig
{

TFeatureConfig::TFeatureConfig(YAML::Node&& root)
    : _root(root)
{}

} // namespace NKVStore::NCore::NConfig
