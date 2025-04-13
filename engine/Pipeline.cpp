#include "Pipeline.h"

namespace engine {
    void Pipeline::addHandler(const std::shared_ptr<Handler>& handler) {
            this->handlers.emplace(handler->getName(), handler);
        }

    void Pipeline::addNext(const std::string& from, const std::string& to) {
        auto fromIt = this->handlers.find(from);
        auto toIt = this->handlers.find(to);

        if (fromIt != this->handlers.end() && toIt != handlers.end()) {
            fromIt->second->addNext(toIt->second);
        } else {
            this->errors.push_back("addNext: Handler not found: " + from + " or " + to);
        }
    }

    void Pipeline::addBranch(const std::string& from,
                   const std::string& to,
                   std::function<bool(const Request&)> condition) {
        auto fromIt = this->handlers.find(from);
        auto toIt = this->handlers.find(to);

        if (fromIt == this->handlers.end() || toIt == this->handlers.end()) {
            this->errors.push_back("addBranch: handler '" + from + "' or '" + to + "' not found.");
            return;
        }

        auto switchHandler = std::dynamic_pointer_cast<SwitchHandler>(fromIt->second);
        if (!switchHandler) {
            this->errors.push_back("addBranch: handler '" + from + "' is not a SwitchHandler.");
            return;
        }

        switchHandler->addBranch(condition, toIt->second);
    }

    void Pipeline::setFirst(const std::string& name) {
        auto it = this->handlers.find(name);
        if (it != this->handlers.end()) {
            this->firstHandler = it->second;
        } else {
            this->errors.push_back("setFirst: handler '" + name + "' not found.");
        }
    }

    void Pipeline::addLast(const std::string& name) {
        auto it = handlers.find(name);
        if (it != handlers.end()) {
            this->lastHandlers.insert(it->second);
        } else {
            this->errors.push_back("addLast: handler '" + name + "' not found.");
        }
    }

    auto Pipeline::isComplete() const -> PipelineValidationResult {
        std::vector<std::string> allErrors = errors;

        if (!firstHandler) {
            allErrors.push_back("Pipeline: no first handler set.");
        }

        if (lastHandlers.empty()) {
            allErrors.push_back("Pipeline: no last handlers defined.");
        }

        for (const auto& [name, handler] : handlers) {
            const bool hasNexts = !handler->getNextHandlers().empty();
            const bool isExplicitLast = lastHandlers.contains(handler);

            const auto* switchHandler = dynamic_cast<const SwitchHandler*>(handler.get());
            bool isSwitchValid = false;

            if (switchHandler) {
                isSwitchValid = !switchHandler->getBranches().empty();
            }

            if (!hasNexts && !isExplicitLast && !isSwitchValid) {
                allErrors.push_back("Handler '" + name + "' has no next handlers and is not marked as last.");
            }
        }

        if (!allErrors.empty()) {
            return { false, allErrors };
        }

        return { true, {} };
    }

    void Pipeline::execute(const Request& request) {
        if (!this->firstHandler) {
            std::cerr << "Pipeline execution failed: no first handler set.\n";
            return;
        }

        std::cout << "< < < Starting pipeline > > >" << std::endl;
        processHandler(this->firstHandler, request);
    }

    auto Pipeline::getFinalResult() const -> std::shared_ptr<Result> {
        return this->finalResult;
    }

    void Pipeline::processHandler(const std::shared_ptr<Handler>& handler, const Request& request) {
        auto futureResult = handler->computeAsync(request);
        auto result = futureResult.get();
        auto prefix = result->get("Stack").empty() ? "" : " -> ";
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

        if (this->lastHandlers.contains(handler)) {
            this->finalResult = result;
        }
    }
}
