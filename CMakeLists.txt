cmake_minimum_required(VERSION 3.10)

# DirectSR-Headers
# Provides DirectSR header, D3D12 Agility SDK exports, and DLL copy utilities.
#
# Targets:
#   DirectSR-Headers      - INTERFACE library providing directsr.h include path
#   DirectSR-AgilitySDK   - OBJECT library exporting D3D12SDKVersion and D3D12SDKPath symbols
#
# Functions:
#   copy_directsr_dlls(TARGET_NAME) - Copies D3D12Core.dll and DirectSR.dll next to the target executable
#
# Usage in any CMake project:
#   FetchContent_Declare(DirectSR-Headers
#       GIT_REPOSITORY git@github.com:MikhailGorobets/DirectSR-Headers.git
#       GIT_TAG        main
#   )
#   FetchContent_MakeAvailable(DirectSR-Headers)
#
#   target_link_libraries(MyApp PRIVATE DirectSR-Headers DirectSR-AgilitySDK)
#   copy_directsr_dlls(MyApp)

set(DIRECTSR_SDK_VERSION 716 CACHE STRING "D3D12 Agility SDK version")
set(DIRECTSR_SDK_PATH    ".\\\\D3D12\\\\" CACHE STRING "D3D12 Agility SDK DLL relative path")

# --- DirectSR-Headers: INTERFACE library for directsr.h ---
add_library(DirectSR-Headers INTERFACE)
target_include_directories(DirectSR-Headers INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# --- DirectSR-AgilitySDK: OBJECT library exporting D3D12SDKVersion / D3D12SDKPath ---
add_library(DirectSR-AgilitySDK OBJECT "${CMAKE_CURRENT_SOURCE_DIR}/src/D3D12AgilitySDK.cpp")
set_target_properties(DirectSR-AgilitySDK PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_compile_definitions(DirectSR-AgilitySDK PRIVATE
    DIRECTSR_D3D12_SDK_VERSION=${DIRECTSR_SDK_VERSION}
    "DIRECTSR_D3D12_SDK_PATH=\"${DIRECTSR_SDK_PATH}\""
)

# --- DLL paths (auto-select x64 vs x86) ---
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(DIRECTSR_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/x64" CACHE INTERNAL "DirectSR DLL directory")
else()
    set(DIRECTSR_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/x86" CACHE INTERNAL "DirectSR DLL directory")
endif()

# --- copy_directsr_dlls(TARGET_NAME) ---
# Copies D3D12Core.dll and DirectSR.dll into <target_exe_dir>/D3D12/
function(copy_directsr_dlls TARGET_NAME)
    if(NOT EXISTS "${DIRECTSR_BIN_DIR}/D3D12Core.dll")
        message(WARNING "DirectSR DLLs not found in '${DIRECTSR_BIN_DIR}'. Skipping copy for ${TARGET_NAME}.")
        return()
    endif()
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${TARGET_NAME}>/D3D12"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DIRECTSR_BIN_DIR}/D3D12Core.dll"
            "$<TARGET_FILE_DIR:${TARGET_NAME}>/D3D12/D3D12Core.dll"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DIRECTSR_BIN_DIR}/DirectSR.dll"
            "$<TARGET_FILE_DIR:${TARGET_NAME}>/D3D12/DirectSR.dll"
        COMMENT "Copying DirectSR Agility SDK DLLs for ${TARGET_NAME}"
    )
endfunction()
