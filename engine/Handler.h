#pragma once

#include "Request.h"
#include "Result.h"

#include <future>
#include <string>
#include <vector>

namespace engine {
    class Handler {
    public:
        Handler(const std::string& name);
        virtual ~Handler() = default;

        auto getName() const -> std::string;

        void addNext(const std::shared_ptr<Handler>& nextHandler);

        [[nodiscard]] auto getNextHandlers() const -> const std::vector<std::shared_ptr<Handler>>&;

        auto compute(const Request& request) -> std::shared_ptr<Result>;
        auto computeAsync(const Request& request) -> std::future<std::shared_ptr<Result>>;

    protected:
        virtual auto doCompute(const Request& request) -> std::shared_ptr<Result> = 0;

    private:
        std::string name;
        std::vector<std::shared_ptr<Handler>> nextHandlers;
    };
}
