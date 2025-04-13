#include "Pipeline.h"

namespace engine {
    void Pipeline::addHandler(const std::shared_ptr<Handler>& handler) {
            handlers.emplace(handler->getName(), handler);
        }

    void Pipeline::addNext(const std::string& from, const std::string& to) {
        auto fromIt = handlers.find(from);
        auto toIt = handlers.find(to);

        if (fromIt != handlers.end() && toIt != handlers.end()) {
            fromIt->second->addNext(toIt->second);
        } else {
            errors.push_back("addNext: Handler not found: " + from + " or " + to);
        }
    }

    void Pipeline::addBranch(const std::string& from,
                   const std::string& to,
                   std::function<bool(const Request&)> condition) {
        auto fromIt = handlers.find(from);
        auto toIt = handlers.find(to);

        if (fromIt == handlers.end() || toIt == handlers.end()) {
            errors.push_back("addBranch: handler '" + from + "' or '" + to + "' not found.");
            return;
        }

        auto switchHandler = std::dynamic_pointer_cast<SwitchHandler>(fromIt->second);
        if (!switchHandler) {
            errors.push_back("addBranch: handler '" + from + "' is not a SwitchHandler.");
            return;
        }

        switchHandler->addBranch(condition, toIt->second);
    }

    void Pipeline::setFirst(const std::string& name) {
        auto it = handlers.find(name);
        if (it != handlers.end()) {
            firstHandler = it->second;
        } else {
            errors.push_back("setFirst: handler '" + name + "' not found.");
        }
    }

    void Pipeline::addLast(const std::string& name) {
        auto it = handlers.find(name);
        if (it != handlers.end()) {
            lastHandlers.insert(it->second);
        } else {
            errors.push_back("addLast: handler '" + name + "' not found.");
        }
    }

    auto Pipeline::isComplete() const -> PipelineValidationResult {
        if (!firstHandler) {
            return { false, { "Pipeline: no first handler set." } };
        }

        if (lastHandlers.empty()) {
            return { false, { "Pipeline: no last handlers defined." } };
        }

        if (!errors.empty()) {
            return { false, errors };
        }

        return { true, {} };
    }

    void Pipeline::execute(const Request& request) {
        if (!firstHandler) {
            std::cerr << "Pipeline execution failed: no first handler set.\n";
            return;
        }

        std::cout << "...Starting pipeline...\n";
        processHandler(firstHandler, request);
    }

    auto Pipeline::getFinalResult() const -> std::shared_ptr<Result> {
        return finalResult;
    }

    void Pipeline::processHandler(const std::shared_ptr<Handler>& handler, const Request& request) {
        auto futureResult = handler->computeAsync(request);
        auto result = futureResult.get();
        auto prefix = result->get("Stack").empty() ? "" : "->";
        result->set("Stack", result->get("Stack") + prefix + handler->getName());

        engine::Request newRequest{
            request.inputType,
            result->fileType,
            result->metadata,
           std::move(result->data)
        };

        if (auto switchHandler = std::dynamic_pointer_cast<SwitchHandler>(handler)) {
            auto next = switchHandler->selectBranch(newRequest);
            if (next) {
                processHandler(next, newRequest);
            } else {
                std::cout << "[Pipeline] No matching branch in switch handler: " << handler->getName() << "\n";
            }
        } else {
            for (const auto& next : handler->getNextHandlers()) {
                processHandler(next, newRequest);
            }
        }

        if (lastHandlers.contains(handler)) {
            finalResult = result;
        }
    }
}
