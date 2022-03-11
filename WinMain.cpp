#include "./Engine/EngineEntry.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {

    EngineEntry entry;
    entry.Initialize(hInstance, "myTitle", "myClass", 800, 600);

    while (entry.Process()) {
        entry.Update();
    }

    return 0;
}
