#pragma once

#include "engine/Handler.h"
#include "engine/Request.h"
#include "engine/Result.h"

namespace handler {
    class HttpParser: public engine::Handler {
    public:
        explicit HttpParser(const std::string& name);

    protected:
        auto doCompute(const engine::Request& request) -> std::shared_ptr<engine::Result> override;
    };
}
