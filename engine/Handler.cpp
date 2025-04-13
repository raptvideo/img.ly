#include "Handler.h"

namespace engine {
    Handler::Handler(const std::string& name) : name(name) {}

    auto Handler::getName() const -> std::string { return name; }

    void Handler::addNext(const std::shared_ptr<Handler>& nextHandler) {
        nextHandlers.push_back(nextHandler);
    }

    auto Handler::getNextHandlers() const -> const std::vector<std::shared_ptr<Handler>>& {
        return nextHandlers;
    }

    auto Handler::compute(const Request& request) -> std::shared_ptr<Result> {
        return this->doCompute(request);
    }

    auto Handler::computeAsync(const Request& request)-> std::future<std::shared_ptr<Result>> {
        return std::async(std::launch::async, [this, request]() {
            return this->doCompute(request);
        });
    }
}
