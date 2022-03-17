#pragma once
#include "../../Generic/PathHelper.h"
#include "GraphicAdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include <d3d11.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>

class Graphics {
  public:
    bool Initialize(HWND hwnd, int width, int height);
    void RenderFrame();

  private:
    ComPtr<ID3D11Device> cptrDevice;
    ComPtr<ID3D11DeviceContext> cptrDeviceContext;
    ComPtr<IDXGISwapChain> cptrSwapchain;
    ComPtr<ID3D11RenderTargetView> cptrRenderTargetView;

    VertexShader vertexshader;

    ComPtr<ID3D11Buffer> cptrVertexBuffer;

    ComPtr<ID3D11DepthStencilView> cptrDepthStencilView;
    ComPtr<ID3D11Texture2D> cptrDepthStencilBuffer;
    ComPtr<ID3D11DepthStencilState> cptrDepthStencilState;

    ComPtr<ID3D11RasterizerState> cptrRasterizerState;

    PixelShader pixelshader;

    unique_ptr<SpriteBatch> uptrSpriteBatch;
    unique_ptr<SpriteFont> uptrSpriteFont;

    ComPtr<ID3D11SamplerState> cptrSamplerState;
    ComPtr<ID3D11ShaderResourceView> cptrMyTexture;

    bool InitializeDirectX(HWND hwnd, int width, int height);
    bool InitializeShaders();
    bool InitializeScene();
    bool AppendVertexBuffer(Vertex *vertices, int size,
                            ID3D11Buffer **ptrVertexBuffer);
};