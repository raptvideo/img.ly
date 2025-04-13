#pragma once

#include "Handler.h"
#include "Request.h"
#include "SwitchHandler.h"

#include <unordered_map>
#include <unordered_set>

namespace engine {

    struct PipelineValidationResult {
        bool success;
        std::vector<std::string> errors;
    };

    class Pipeline {
    public:
        void addHandler(const std::shared_ptr<Handler>& handler);

        void addNext(const std::string& from, const std::string& to);

        void addBranch(const std::string& from,
                       const std::string& to,
                       std::function<bool(const Request&)> condition);

        void setFirst(const std::string& name);

        void addLast(const std::string& name);

        auto isComplete() const -> PipelineValidationResult;

        void execute(const Request& request);

        auto getFinalResult() const -> std::shared_ptr<Result>;

    private:
        void processHandler(const std::shared_ptr<Handler>& handler, const Request& request);

        std::unordered_map<std::string, std::shared_ptr<Handler>> handlers;
        std::shared_ptr<Handler> firstHandler;
        std::unordered_set<std::shared_ptr<Handler>> lastHandlers;

        std::shared_ptr<Result> finalResult;

        std::vector<std::string> errors;
    };
}
