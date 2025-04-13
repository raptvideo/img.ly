#pragma once

#include "engine/Handler.h"
#include "engine/Request.h"
#include "engine/Result.h"

#include <iostream>
#include <string>

namespace handler {
    class FileParser: public engine::Handler {
    public:
        explicit FileParser(const std::string& name);

    protected:
        auto doCompute(const engine::Request& request) -> std::shared_ptr<engine::Result> override;
    };
}
