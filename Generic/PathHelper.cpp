#include "PathHelper.h"

wstring PathHelper::GetEnvironmentDir() {
    wstring folder = L"";
#pragma region DetermineShaderPath
    if (IsDebuggerPresent() == TRUE) {
// DEBUG MODE
#ifdef _DEBUG
// X64
#ifdef _WIN64
        folder = L"x64/Debug/";
// X86
#else
        folder = L"Debug/";
#endif
// RELEASE MODE
#else
// X64
#ifdef _WIN64
        folder = L"x64/Release/";
#else
        folder = L"Release/";
#endif
#endif
    }
    return folder;
}
