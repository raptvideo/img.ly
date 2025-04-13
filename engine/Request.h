#pragma once

#include <iostream>
#include <variant>
#include <vector>
#include <unordered_map>
#include <any>

namespace engine {
    enum class InputType {
        File,
        Http,
        Bundle
    };

    enum class BaseFileType {
        Image,
        Json
    };

    struct FileType {
        enum class WrapperType { Base, Compress };

        WrapperType type;
        std::variant<BaseFileType, std::shared_ptr<FileType>> inner;

        static auto base(BaseFileType base) -> FileType;

        static auto compress(FileType inner) -> FileType;

        bool operator==(const FileType & file) const = default;

        [[nodiscard]] bool isCompress() const;
    };

    struct Request {
        InputType inputType;
        FileType fileType;
        std::unordered_map<std::string, std::string> metadata;
        std::any data;
    };
}
