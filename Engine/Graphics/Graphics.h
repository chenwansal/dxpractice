#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "GraphicAdapterReader.h"

class Graphics {
  public:
    bool Initialize(HWND hwnd, int width, int height);
    void RenderFrame();

  private:
    ComPtr<ID3D11Device> ptrDevice;
    ComPtr<ID3D11DeviceContext> ptrDeviceContext;
    ComPtr<IDXGISwapChain> ptrSwapchain;
    ComPtr<ID3D11RenderTargetView> ptrRenderTargetView;

    bool InitializeDirectX(HWND hwnd, int width, int height);
};