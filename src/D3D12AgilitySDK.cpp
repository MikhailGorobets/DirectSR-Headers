/*
 *  D3D12 Agility SDK exports.
 *
 *  These symbols are detected by the Windows D3D12 runtime at DLL load time,
 *  causing it to load D3D12Core.dll (and DirectSR.dll) from the path specified
 *  by D3D12SDKPath, relative to the executable directory.
 *
 *  DIRECTSR_D3D12_SDK_VERSION and DIRECTSR_D3D12_SDK_PATH are defined via
 *  target_compile_definitions in CMakeLists.txt.
 */

#include <cstdint>

extern "C"
{
    __declspec(dllexport) extern const uint32_t D3D12SDKVersion = DIRECTSR_D3D12_SDK_VERSION;
}

extern "C"
{
    __declspec(dllexport) extern const char* D3D12SDKPath = DIRECTSR_D3D12_SDK_PATH;
}
