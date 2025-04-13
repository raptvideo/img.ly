#include "SwitchHandler.h"
#include <iostream>

namespace engine {
    SwitchHandler::SwitchHandler(const std::string& name)
        : Handler(name) {}

    void SwitchHandler::addBranch(std::function<bool(const Request&)> condition,
                                   const std::shared_ptr<Handler>& nextHandler) {
        branches.emplace_back(std::move(condition), nextHandler);
    }

    auto SwitchHandler::selectBranch(const Request& request) const -> std::shared_ptr<Handler> {
        for (const auto& [condition, handler] : branches) {
            if (condition(request)) {
                return handler;
            }
        }
        return nullptr;
    }

    auto SwitchHandler::doCompute(const Request& request) -> std::shared_ptr<Result> {
        auto result = std::make_shared<engine::Result>();
        result->status = engine::Status::Success;
        result->fileType = request.fileType;
        result->metadata = request.metadata;
        result->data = request.data;
        result->set("handler", this->getName());

        std::cout << "[SwitchHandler] Executing: " << getName() << "\n";
        return result;
    }
}
