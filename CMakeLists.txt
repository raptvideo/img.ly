cmake_minimum_required(VERSION 3.30)
project(img_ly)

set(CMAKE_CXX_STANDARD 20)

add_executable(img_ly main.cpp
        engine/Pipeline.cpp
        engine/Pipeline.h
        engine/Handler.cpp
        engine/Handler.h
        engine/Request.cpp
        engine/Request.h
        engine/Result.cpp
        engine/Result.h
        engine/SwitchHandler.cpp
        engine/SwitchHandler.h
        handlers/FileParser.cpp
        handlers/FileParser.h
        handlers/HttpParser.cpp
        handlers/HttpParser.h
        handlers/BundleParser.cpp
        handlers/BundleParser.h
        handlers/ImageDecoder.cpp
        handlers/ImageDecoder.h
        handlers/ZipDecompressor.cpp
        handlers/ZipDecompressor.h
        handlers/JsonParser.cpp
        handlers/JsonParser.h)

target_include_directories(img_ly PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
