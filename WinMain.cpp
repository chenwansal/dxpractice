#include "./Engine/EngineCore.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {

    EngineCore engine;
    if (engine.Initialize(hInstance, "myTitle", "myClass", 800, 800)) {
        while (engine.Process()) {
            engine.Update();
            engine.RenderFrame();
        }
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