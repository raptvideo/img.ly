#include "engine/Pipeline.h"
#include "engine/Request.h"
#include "engine/Result.h"
#include "engine/SwitchHandler.h"
#include "handlers/BundleParser.h"
#include "handlers/FileParser.h"
#include "handlers/HttpParser.h"
#include "handlers/ImageDecoder.h"
#include "handlers/JsonParser.h"
#include "handlers/ZipDecompressor.h"

#include <cassert>

int main() {
    engine::Pipeline pipeline;

    auto checkInput = std::make_shared<engine::SwitchHandler>("CheckInput");
    pipeline.addHandler(checkInput);

    auto bundleParser = std::make_shared<handler::BundleParser>("BundleParser");
    pipeline.addHandler(bundleParser);
    auto fileParser = std::make_shared<handler::FileParser>("FileParser");
    pipeline.addHandler(fileParser);
    auto httpParser = std::make_shared<handler::HttpParser>("HttpParser");
    pipeline.addHandler(httpParser);

    auto checkFileType = std::make_shared<engine::SwitchHandler>("CheckFileType");
    pipeline.addHandler(checkFileType);

    auto imageDecoder = std::make_shared<handler::ImageDecoder>("ImageDecoder");
    pipeline.addHandler(imageDecoder);
    auto jsonParser = std::make_shared<handler::JsonParser>("JsonParser");
    pipeline.addHandler(jsonParser);
    auto zipDecompressor = std::make_shared<handler::ZipDecompressor>("ZipDecompressor");
    pipeline.addHandler(zipDecompressor);

    pipeline.setFirst("CheckInput");
    pipeline.addBranch("CheckInput", "BundleParser", [](const engine::Request& req) { return req.inputType == engine::InputType::Bundle; });
    pipeline.addBranch("CheckInput", "FileParser", [](const engine::Request& req) { return req.inputType == engine::InputType::File; });
    pipeline.addBranch("CheckInput", "HttpParser", [](const engine::Request& req) { return req.inputType == engine::InputType::Http; });

    pipeline.addNext("BundleParser", "CheckFileType");
    pipeline.addNext("FileParser", "CheckFileType");
    pipeline.addNext("HttpParser", "CheckFileType");

    pipeline.addBranch("CheckFileType", "ImageDecoder", [](const engine::Request& req) { return req.fileType == engine::FileType::base(engine::BaseFileType::Image); });
    pipeline.addBranch("CheckFileType", "JsonParser", [](const engine::Request& req) { return req.fileType == engine::FileType::base(engine::BaseFileType::Json); });
    pipeline.addBranch("CheckFileType", "ZipDecompressor", [](const engine::Request& req) { return req.fileType.isCompress(); });

    pipeline.addNext("ZipDecompressor", "CheckFileType");

    pipeline.addLast("ImageDecoder");
    pipeline.addLast("JsonParser");

    assert(pipeline.isComplete().success);

    engine::Request request1{
        engine::InputType::File,
        engine::FileType::compress(engine::FileType::compress(engine::FileType::base(engine::BaseFileType::Image))),
        {}
    };

    pipeline.execute(request1);
    auto result = pipeline.getFinalResult();
    assert(result->get("Stack") == "CheckInput->FileParser->CheckFileType->ZipDecompressor->CheckFileType->ZipDecompressor->CheckFileType->ImageDecoder");

    engine::Request request2{
        engine::InputType::Http,
        engine::FileType::compress(engine::FileType::base(engine::BaseFileType::Json)),
        {}
    };

    pipeline.execute(request2);
    result = pipeline.getFinalResult();
    assert(result->get("Stack") == "CheckInput->HttpParser->CheckFileType->ZipDecompressor->CheckFileType->JsonParser");

    engine::Request request3{
        engine::InputType::Bundle,
        engine::FileType::base(engine::BaseFileType::Image),
        {}
    };

    pipeline.execute(request3);
    result = pipeline.getFinalResult();
    assert(result->get("Stack") == "CheckInput->BundleParser->CheckFileType->ImageDecoder");
}