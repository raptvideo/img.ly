#pragma once

#include "Request.h"

#include <string>
#include <unordered_map>
#include <any>

namespace engine {
    enum class Status {
        Success,
        Failure
    };

    class Result {
    public:
        Status status;
        std::unordered_map<std::string, std::string> metadata;
        FileType fileType;
        std::any data;

        void set(const std::string& key, const std::string& value);

        [[nodiscard]] auto get(const std::string& key) const -> std::string;
    };
}
