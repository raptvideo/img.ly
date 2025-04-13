#pragma once

#include "engine/Handler.h"

#include <iostream>
#include <string>

namespace handler {
    class ImageDecoder: public engine::Handler {
    public:
        explicit ImageDecoder(const std::string& name);

    protected:
        auto doCompute(const engine::Request& request) -> std::shared_ptr<engine::Result> override;
    };
}
