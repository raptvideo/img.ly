#include "BundleParser.h"

namespace handler {
    BundleParser::BundleParser(const std::string& name) : engine::Handler(name) {}

    auto BundleParser::doCompute(const engine::Request& request) -> std::shared_ptr<engine::Result> {
        auto result = std::make_shared<engine::Result>();
        result->status = engine::Status::Success;
        result->fileType = request.fileType;
        result->metadata = request.metadata;
        result->data = request.data;

        std::cout << "[BundleParser] Executing: " << this->getName() << std::endl;
        return result;
    }
}
