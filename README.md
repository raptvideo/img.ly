# IMG.LY Codding Challenge

This is an asynchronous, modular image-processing pipeline written in modern C++20. It supports complex, branching workflows based on input types and file types, using a declarative structure built around composable Handlers and runtime-evaluated SwitchHandlers.

## Features

- Async handlers with `std::future`
- Smart `SwitchHandler` branching

## Overview

```
                                                                      ------<------------<-----------<---------
                                                                     |                                         |
                        -- (if file://): FileParser     ->           |            -- (if Image): DecodeImage --|-> Process Image ->
                       |                                  |          V           |                             |                   |
Input -> Check Input -> -- (if http://): HttpParser     -> --  Check File Type -> -- (if Zip): DecompressZip ->                    |--Success
                       |                                  |                      |                                                 |
                        -- (if bundle://): BundleParser ->                        -- (if Json): ParseJson -------> Process Json  ->
```
## Core Components

Pipeline — Orchestrates handler execution and branching

Handler — Base class for asynchronous units of work

SwitchHandler — Handles conditional branching based on runtime values

Request — Input to the pipeline, includes inputType, fileType, and std::any data

Result — Output of each handler, shared across stages

Built-in Handlers: FileParser, HttpParser, BundleParser, ImageDecoder, JsonParser - Mock classes

ZipDecompressor - Decompresses compressed files to be further processed

## Project Structure
```
img_ly/
├── engine/
│   ├── Pipeline.h/cpp
│   ├── Handler.h/cpp
│   ├── SwitchHandler.h/cpp
│   ├── Request.h/cpp
│   └── Result.h/cpp
├── handlers/
│   ├── FileParser.h/cpp
│   ├── HttpParser.h/cpp
│   ├── BundleParser.h/cpp
│   ├── ImageDecoder.h/cpp
│   ├── JsonParser.h/cpp
│   └── ZipDecompressor.h/cpp
└── main.cpp
```

## Build & Run

Requires: C++20, CMake ≥ 3.30

```bash
mkdir build && cd build
cmake ..
make
./img_ly
```

## Examples or Test Cases

These are defined in `main.cpp`:

- `file + Compress(Compress(Image))` → unzip twice → decode image
- `http + Compress(Json)` → unzip → parse json
- `bundle + Image` → decode image directly

