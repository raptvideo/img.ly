#include "Result.h"

namespace engine {
    void Result::set(const std::string& key, const std::string& value) {
        metadata[key] = value;
    }

    auto Result::get(const std::string& key) const -> std::string {
        auto it = metadata.find(key);
        return it != metadata.end() ? it->second : "";
    }
}
