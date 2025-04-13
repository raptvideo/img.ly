#include "Request.h"

namespace engine {
    auto FileType::base(BaseFileType base) -> FileType {
        return FileType{WrapperType::Base, base};
    }

    auto FileType::compress(FileType inner) -> FileType {
        return FileType{WrapperType::Compress, std::make_shared<FileType>(std::move(inner))};
    }

    bool FileType::isCompress() const {
        return this->type == WrapperType::Compress;
    }
}
