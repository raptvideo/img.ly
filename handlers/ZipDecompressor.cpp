#include "ZipDecompressor.h"

namespace handler {
    ZipDecompressor::ZipDecompressor(const std::string& name) : engine::Handler(name) {}

    auto ZipDecompressor::doCompute(const engine::Request& request) -> std::shared_ptr<engine::Result> {
        auto result = std::make_shared<engine::Result>();
        result->fileType = request.fileType;
        result->metadata = request.metadata;
        result->data = request.data;

        std::cout << "[ZipDecompressor] Executing: " << getName() << std::endl;

        if (request.fileType.isCompress()) {
            result->status = engine::Status::Success;
            const auto& inner = std::get<std::shared_ptr<engine::FileType>>(request.fileType.inner);

            result->fileType = *inner;
        } else {
            result->status = engine::Status::Failure;
        }

        return result;
    }
}
