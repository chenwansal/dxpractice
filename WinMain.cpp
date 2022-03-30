#include "./Engine/EngineCore.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "Failed To Init WICLOADER");
        return -1;
    }

    EngineCore engine;
    try {
        if (engine.Initialize(hInstance, "myTitle", "myClass", 800, 800)) {
            while (engine.Process()) {
                engine.Update();
                engine.RenderFrame();
            }
        }
    } catch (COMException &ex) {
        PLogger::PopupException(ex);
    }
    return 0;
}

// INPUT ASSEMBLER
// VERTEX SHADER
// RASTERIZER
// PIXEL SHADER
// OUTPUT MERGER

// CREATE VERTEX BUFFER
// DRAW