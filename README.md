# DirectSR-Headers

DirectSR header and D3D12 Agility SDK binaries for [DirectSR](https://devblogs.microsoft.com/directx/directsr/) hardware super resolution.

## Contents

```
include/
  directsr.h          # DirectSR API header (MIDL-generated)
bin/
  x64/
    D3D12Core.dll     # Agility SDK runtime (x64)
    DirectSR.dll      # DirectSR implementation (x64)
  x86/
    D3D12Core.dll     # Agility SDK runtime (x86)
    DirectSR.dll      # DirectSR implementation (x86)
src/
  D3D12AgilitySDK.cpp # Exports D3D12SDKVersion and D3D12SDKPath symbols
```

## CMake Integration

### With FetchContent (recommended)

```cmake
include(FetchContent)
FetchContent_Declare(DirectSR-Headers
    GIT_REPOSITORY git@github.com:MikhailGorobets/DirectSR-Headers.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(DirectSR-Headers)
```

### As a subdirectory

```cmake
add_subdirectory(DirectSR-Headers)
```

## Usage

This package provides three components:

| Component | Type | Purpose |
|---|---|---|
| `DirectSR-Headers`     | INTERFACE library | Adds `include/` to the include path, providing `<directsr.h>` |
| `DirectSR-AgilitySDK`  | OBJECT library | Exports `D3D12SDKVersion` and `D3D12SDKPath` symbols required by the Agility SDK runtime |
| `copy_directsr_dlls()` | CMake function | Copies `D3D12Core.dll` and `DirectSR.dll` to the target executable's output directory |


### Complete minimal example

```cmake
cmake_minimum_required(VERSION 3.11)
project(MyApp CXX)

include(FetchContent)
FetchContent_Declare(DirectSR-Headers
    GIT_REPOSITORY git@github.com:MikhailGorobets/DirectSR-Headers.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(DirectSR-Headers)

add_executable(MyApp main.cpp)
target_link_libraries(MyApp PRIVATE DirectSR-Headers DirectSR-AgilitySDK)
copy_directsr_dlls(MyApp)
```

## How it works

The D3D12 Agility SDK requires two things from an application:

1. **Exported symbols** — `D3D12SDKVersion` (uint32) and `D3D12SDKPath` (char8_t*) must be exported from the executable. The Windows D3D12 runtime detects these at load time and loads `D3D12Core.dll` from the specified path instead of the system version.

2. **DLL files** — `D3D12Core.dll` and `DirectSR.dll` must be placed in a subfolder (default: `D3D12\`) relative to the executable.

`DirectSR-AgilitySDK` handles (1) and `copy_directsr_dlls()` handles (2).
