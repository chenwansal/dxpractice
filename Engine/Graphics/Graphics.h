#pragma once
#include <d3d11.h>
#include "GraphicAdapterReader.h"
#include "Shaders.h"
#include "../../Generic/PathHelper.h"
#include "Vertex.h"

class Graphics {
  public:
    bool Initialize(HWND hwnd, int width, int height);
    void RenderFrame();

  private:
    ComPtr<ID3D11Device> ptrDevice;
    ComPtr<ID3D11DeviceContext> ptrDeviceContext;
    ComPtr<IDXGISwapChain> ptrSwapchain;
    ComPtr<ID3D11RenderTargetView> ptrRenderTargetView;
    ComPtr<ID3D11Buffer> ptrVertexBuffer;

    VertexShader vertexshader;
    PixelShader pixelshader;

    bool InitializeDirectX(HWND hwnd, int width, int height);
    bool InitializeShaders();
    bool InitializeScene();
};