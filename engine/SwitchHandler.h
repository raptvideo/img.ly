#pragma once

#include "Handler.h"
#include "Request.h"

#include <functional>
#include <vector>

namespace engine {

    class SwitchHandler : public Handler {
    public:
        explicit SwitchHandler(const std::string& name);
        ~SwitchHandler() override = default;

        void addBranch(std::function<bool(const Request&)> condition,
                       const std::shared_ptr<Handler>& nextHandler);

        [[nodiscard]] auto selectBranch(const Request& request) const -> std::shared_ptr<Handler>;

    protected:
        auto doCompute(const Request& request) -> std::shared_ptr<Result> override;

    private:
        std::vector<std::pair<std::function<bool(const Request&)>, std::shared_ptr<Handler>>> branches;
    };
}
