#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int height) {
    if (!InitializeDirectX(hwnd, width, height)) {
        PLogger::ConsoleLog("false");
        return false;
    }
    return true;
}

bool Graphics::InitializeDirectX(HWND hwnd, int width, int height) {
    // HRESULT hr = D3D11CreateDeviceAndSwapChain()
    vector<GraphicAdapterData> adapters =
        GraphicAdapterReader::GetAdapterDatas();
    return true;
}