#include "Graphics.h"

// public instance
bool Graphics::Initialize(HWND hwnd, int width, int height) {

    this->windowWidth = width;
    this->windowHeight = height;

    if (!InitializeDirectX(hwnd)) {
        return false;
    }

    if (!InitializeShaders()) {
        return false;
    }

    if (!InitializeScene()) {
        return false;
    }

    return true;
}

void Graphics::RenderFrame() {

    float bgcolor[] = {0.1f, 0.1f, 0.1f, 1.0f};

    // CLEAR RENDER TARGET VIEW
    this->cptrDeviceContext->ClearRenderTargetView(
        this->cptrRenderTargetView.Get(), bgcolor);

    // CLEAR DEPTH STENCIL VIEW
    this->cptrDeviceContext->ClearDepthStencilView(
        this->cptrDepthStencilView.Get(),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // SET INPUT LAYOUT
    this->cptrDeviceContext->IASetInputLayout(
        this->vertexshader.GetInputLayout());

    // SET PRIMITIVE TOPOLOGY
    this->cptrDeviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // SET RASTERIZER STATE
    this->cptrDeviceContext->RSSetState(this->cptrRasterizerState.Get());

    // SET OUTPUT MERGER STENCIL STATE
    this->cptrDeviceContext->OMSetDepthStencilState(
        this->cptrDepthStencilState.Get(), 0);

    // SET PS Sampler
    this->cptrDeviceContext->PSSetSamplers(
        0, 1, this->cptrSamplerState.GetAddressOf());

    // SET VERTEX SHADER
    this->cptrDeviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);

    // SET PIXEL SHADER
    this->cptrDeviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);

    // SET VERTEX BUFFERS
    // AND DRAW
    UINT offset = 0;

    // Update Constant Buffer
    constantBuffer.data.mat =
        XMMatrixRotationRollPitchYaw(0, 0, XM_PIDIV4);
    //constantBuffer.data.mat = XMMatrixTranslation(0, 0.5f, 0);
    constantBuffer.data.mat = XMMatrixTranspose(constantBuffer.data.mat);
    if (!constantBuffer.ApplyChanges(this->cptrDeviceContext.Get())) {
        return;
    }
    this->cptrDeviceContext->VSSetConstantBuffers(
        0, 1, constantBuffer.GetAddressOf());

    // DRAW SQUARE
    this->cptrDeviceContext->PSSetShaderResources(
        0, 1, this->cptrMyTexture.GetAddressOf());
    this->cptrDeviceContext->IASetVertexBuffers(
        0, 1, this->vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(),
        &offset);
    this->cptrDeviceContext->IASetIndexBuffer(this->indexBuffer.Get(),
                                              DXGI_FORMAT_R32_UINT, 0);
    this->cptrDeviceContext->DrawIndexed(this->indexBuffer.BufferSize(), 0, 0);

    // DRAW TEXT
    uptrSpriteBatch->Begin();
    uptrSpriteFont->DrawString(uptrSpriteBatch.get(), L"hello pig",
                               XMFLOAT2(0.0f, 0.0f), Colors::White, 0.0f,
                               XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));
    uptrSpriteBatch->End();

    // SWAP CHAIN
    this->cptrSwapchain->Present(1, NULL);
}

// private instance
bool Graphics::InitializeDirectX(HWND hwnd) {

    // DEVICE AND SWAPCHAIN
    vector<GraphicAdapterData> adapters =
        GraphicAdapterReader::GetAdapterDatas();

    if (adapters.size() < 1) {
        PLogger::PopupError("NO IDXGI ADAPTER");
        return false;
    }

    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferDesc.Width = this->windowWidth;
    scd.BufferDesc.Height = this->windowHeight;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;

    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 1;
    scd.OutputWindow = hwnd;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    HRESULT hr;
    hr = D3D11CreateDeviceAndSwapChain(
        adapters[0].ptrAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, NULL, NULL, 0,
        D3D11_SDK_VERSION, &scd, this->cptrSwapchain.GetAddressOf(),
        this->cptrDevice.GetAddressOf(), NULL,
        this->cptrDeviceContext.GetAddressOf());

    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr,
                                      "FAILED CREATE DEVICE AND SWAP CHAIN");
        return false;
    }

    ComPtr<ID3D11Texture2D> backBuffer;
    hr = this->cptrSwapchain->GetBuffer(
        0, __uuidof(ID3D11Texture2D),
        reinterpret_cast<void **>(backBuffer.GetAddressOf()));

    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "GETBUFFER FAILED");
        return false;
    }

    // RENDER TARGET VIEW
    hr = this->cptrDevice->CreateRenderTargetView(
        backBuffer.Get(), NULL, this->cptrRenderTargetView.GetAddressOf());

    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "FAILED CERATE RENDER TARGET");
        return false;
    }

    // Describe: Depth/Stencil Buffer
    D3D11_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = this->windowWidth;
    textureDesc.Height = this->windowHeight;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    // CREATE TEXTURE2D
    hr = this->cptrDevice->CreateTexture2D(
        &textureDesc, NULL, this->cptrDepthStencilBuffer.GetAddressOf());
    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "FAILED CERATE DEPTH STENCIL BUFFER");
        return false;
    }

    // CREATE DEPTH STENCIL VIEW
    hr = this->cptrDevice->CreateDepthStencilView(
        this->cptrDepthStencilBuffer.Get(), NULL,
        this->cptrDepthStencilView.GetAddressOf());
    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "FAILED CERATE DEPTH STENCIL VIEW");
        return false;
    }

    // SET OUTPUT MERGER
    this->cptrDeviceContext->OMSetRenderTargets(
        1, this->cptrRenderTargetView.GetAddressOf(),
        this->cptrDepthStencilView.Get());

    // CREATE DEPTH STENCIL STATE
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask =
        D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc =
        D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    hr = this->cptrDevice->CreateDepthStencilState(
        &depthStencilDesc, this->cptrDepthStencilState.GetAddressOf());
    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "FAILED CERATE DEPTH STENCIL STATE");
        return false;
    }

    // CREATE VIEWPORT
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)windowWidth;
    viewport.Height = (float)windowHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    // Set Viewport
    this->cptrDeviceContext->RSSetViewports(1, &viewport);

    // Create Rasterizer State
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
    rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    hr = this->cptrDevice->CreateRasterizerState(
        &rasterizerDesc, this->cptrRasterizerState.GetAddressOf());
    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "FAILED TO CREATE RASTERIZER STATE");
        return false;
    }

    // SET FONTS
    uptrSpriteBatch = make_unique<SpriteBatch>(this->cptrDeviceContext.Get());
    uptrSpriteFont = make_unique<SpriteFont>(
        this->cptrDevice.Get(), L"Data/SpriteFonts/songti_16.sfont");

    // Create Sampler Description
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = this->cptrDevice->CreateSamplerState(
        &samplerDesc, this->cptrSamplerState.GetAddressOf());
    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "Failed To Create Sampler State");
        return false;
    }

    return true;
}

bool Graphics::InitializeShaders() {

    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
         D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0,
         D3D11_APPEND_ALIGNED_ELEMENT,
         D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}};
    UINT numElements = ARRAYSIZE(layoutDesc);

    wstring path = PathHelper::GetEnvironmentDir();
    if (!vertexshader.Initialize(cptrDevice, path + L"vertexshader.cso",
                                 layoutDesc, numElements)) {
        return false;
    }

    if (!pixelshader.Initialize(cptrDevice, path + L"pixelshader.cso",
                                layoutDesc, numElements)) {
        return false;
    }

    return true;
}

bool Graphics::InitializeScene() {

    // Vertices Square
    Vertex v[] = {
        Vertex(-0.5f, -0.5f, 1.0f, 0, 1), // BOTTOM LEFT - [0]
        Vertex(-0.5f, 0.5f, 1.0f, 0, 0),  // TOP LEFT - [1]
        Vertex(0.5f, 0.5f, 1.0f, 1, 0),   // TOP RIGHT - [2]
        Vertex(0.5f, -0.5f, 1.0f, 1, 1),  // BOTTOM RIGHT [3]
    };

    // Load Vertex Buffer
    HRESULT hr =
        this->vertexBuffer.Initialize(this->cptrDevice.Get(), v, ARRAYSIZE(v));
    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "FAILED TO CREATE BUFFER");
        return false;
    }

    // Load Index Data
    DWORD indices[] = {0, 1, 2, 0, 2, 3};

    hr = this->indexBuffer.Initialize(this->cptrDevice.Get(), indices,
                                      ARRAYSIZE(indices));
    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "FAILED TO INDICES BUFFER");
        return false;
    }

    // Load Texture
    hr = CreateWICTextureFromFile(this->cptrDevice.Get(),
                                  L"Data/Textures/avatar.jpg", nullptr,
                                  this->cptrMyTexture.GetAddressOf());
    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "FAILED TO CREATE WIC Texture");
        return false;
    }

    // Initialize Constant Buffer
    hr = this->constantBuffer.Initialize(this->cptrDevice.Get());
    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "FAILED TO CREATE CONSTANT BUFFER");
        return false;
    }

    return true;
}