#pragma once

#include "engine/Handler.h"
#include "engine/Request.h"
#include "engine/Result.h"

#include <iostream>
#include <string>

namespace handler {
    class ZipDecompressor: public engine::Handler {
    public:
        explicit ZipDecompressor(const std::string& name);

    protected:
        auto doCompute(const engine::Request& request) -> std::shared_ptr<engine::Result> override;
    };
}
