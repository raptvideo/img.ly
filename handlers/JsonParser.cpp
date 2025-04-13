#include "JsonParser.h"

namespace handler {
    JsonParser::JsonParser(const std::string& name) : engine::Handler(name) {}

    auto JsonParser::doCompute(const engine::Request& request) -> std::shared_ptr<engine::Result> {
        auto result = std::make_shared<engine::Result>();
        result->status = engine::Status::Success;
        result->fileType = request.fileType;
        result->metadata = request.metadata;
        result->data = request.data;

        std::cout << "[JsonParser] Executing: " << getName() << "\n";
        return result;
    }
}
