#pragma once

#include "engine/Handler.h"
#include "engine/Request.h"
#include "engine/Result.h"

#include <iostream>
#include <string>

#include "../engine/Result.h"

namespace handler {
    class BundleParser: public engine::Handler {
    public:
        explicit BundleParser(const std::string& name);

    protected:
        auto doCompute(const engine::Request& request) -> std::shared_ptr<engine::Result> override;
    };
}
