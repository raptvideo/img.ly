#include "Result.h"

namespace engine {
    void Result::set(const std::string& key, const std::string& value) {
        this->metadata[key] = value;
    }

    auto Result::get(const std::string& key) const -> std::string {
        auto it = this->metadata.find(key);
        return it != this->metadata.end() ? it->second : "";
    }
}
