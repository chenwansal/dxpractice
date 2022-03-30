#pragma once
#include "../../Generic/PathHelper.h"
#include "../ImGUI/imgui.h"
#include "../ImGUI/imgui_impl_dx11.h"
#include "../ImGUI/imgui_impl_win32.h"
#include "../Timer/Timer.h"
#include "Adapter/GraphicAdapterReader.h"
#include "Camera/Camera.h"
#include "Model/Model.h"
#include "Shader/ConstantBufferTypes.h"
#include "Shader/Shaders.h"
#include "ShaderBuffer/ConstantBuffer.h"
#include "ShaderBuffer/IndexBuffer.h"
#include "ShaderBuffer/VertexBuffer.h"
#include "Vertex/Vertex.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include <d3d11.h>

class Graphics {
  public:
    Camera camera;
    Model model;

    bool Initialize(HWND hwnd, int width, int height);
    void RenderFrame();

  private:
    int windowWidth;
    int windowHeight;

    ComPtr<ID3D11Device> cptrDevice;
    ComPtr<ID3D11DeviceContext> cptrDeviceContext;
    ComPtr<IDXGISwapChain> cptrSwapchain;
    ComPtr<ID3D11RenderTargetView> cptrRenderTargetView;

    VertexShader vertexshader;

    ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshaderBuffer;
    ConstantBuffer<CB_PS_pixelshader> cb_ps_pixelshaderBuffer;

    ComPtr<ID3D11DepthStencilView> cptrDepthStencilView;
    ComPtr<ID3D11Texture2D> cptrDepthStencilBuffer;
    ComPtr<ID3D11DepthStencilState> cptrDepthStencilState;

    ComPtr<ID3D11RasterizerState> cptrRasterizerState;
    ComPtr<ID3D11BlendState> cptrBlendstate;

    PixelShader pixelshader;

    unique_ptr<SpriteBatch> uptrSpriteBatch;
    unique_ptr<SpriteFont> uptrSpriteFont;

    ComPtr<ID3D11SamplerState> cptrSamplerState;
    ComPtr<ID3D11ShaderResourceView> cptrPinkTexture;

    Timer fpsTimer;

    bool InitializeDirectX(HWND hwnd);
    bool InitializeShaders();
    bool InitializeScene();
};