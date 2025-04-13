#include "FileParser.h"

namespace handler {
    FileParser::FileParser(const std::string& name) : engine::Handler(name) {}

    auto FileParser::doCompute(const engine::Request& request) -> std::shared_ptr<engine::Result> {
        auto result = std::make_shared<engine::Result>();
        result->status = engine::Status::Success;
        result->fileType = request.fileType;
        result->metadata = request.metadata;
        result->data = request.data;

        std::cout << "[FileParser] Executing: " << getName() << std::endl;
        return result;
    }
}
