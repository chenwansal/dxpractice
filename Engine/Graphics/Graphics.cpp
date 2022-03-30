#include "Graphics.h"

// public instance
bool Graphics::Initialize(HWND hwnd, int width, int height) {

    this->fpsTimer.Start();

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

    // IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(this->cptrDevice.Get(), this->cptrDeviceContext.Get());
    ImGui::StyleColorsDark();

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

    // SET BLEND STATE
    this->cptrDeviceContext->OMSetBlendState(this->cptrBlendstate.Get(), NULL,
                                             0xFFFFFFFF);

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

    /* WE MUST DRAW OPAQUE FIRST */
    /* WE MUST DRAW OPAQUE FIRST */
    /* WE MUST DRAW OPAQUE FIRST */
    static float alpha = 1.0f;
    { // PINK
        // ==== Update Constant Buffer ====
        // - VS CONSTANT BUFFER
        static float translationOffset[3] = {0, 0, -0.5f};
        XMMATRIX worldMat = XMMatrixTranslation(
            translationOffset[0], translationOffset[1], translationOffset[2]);
        cb_vs_vertexshaderBuffer.data.mat =
            worldMat * camera.GetViewMatrix() * camera.GetProjectionMatrix();
        cb_vs_vertexshaderBuffer.data.mat =
            XMMatrixTranspose(cb_vs_vertexshaderBuffer.data.mat);
        if (!cb_vs_vertexshaderBuffer.ApplyChanges(
                this->cptrDeviceContext.Get())) {
            return;
        }
        this->cptrDeviceContext->VSSetConstantBuffers(
            0, 1, cb_vs_vertexshaderBuffer.GetAddressOf());

        // - PS CONSTANT BUFFER
        this->cb_ps_pixelshaderBuffer.data.alpha = alpha;
        this->cb_ps_pixelshaderBuffer.ApplyChanges(
            this->cptrDeviceContext.Get());
        this->cptrDeviceContext->PSSetConstantBuffers(
            0, 1, this->cb_ps_pixelshaderBuffer.GetAddressOf());

        // DRAW SQUARE
        this->cptrDeviceContext->PSSetShaderResources(
            0, 1, this->cptrPinkTexture.GetAddressOf());
        this->cptrDeviceContext->IASetVertexBuffers(
            0, 1, this->vertexBuffer.GetAddressOf(),
            this->vertexBuffer.StridePtr(), &offset);
        this->cptrDeviceContext->IASetIndexBuffer(this->indexBuffer.Get(),
                                                  DXGI_FORMAT_R32_UINT, 0);
        this->cptrDeviceContext->DrawIndexed(this->indexBuffer.BufferSize(), 0,
                                             0);
    }

    // DRAW TEXT
    static int fpsCounter = 0;
    static string fpsString = "FPS: 0";
    fpsCounter += 1;
    if (fpsTimer.GetMillisecondsElapsed() > 1000.0f) {
        fpsString = "FPS: " + to_string(fpsCounter);
        fpsCounter = 0;
        fpsTimer.Restart();
    }
    uptrSpriteBatch->Begin();
    uptrSpriteFont->DrawString(uptrSpriteBatch.get(),
                               StringConverter::StringToWide(fpsString).c_str(),
                               XMFLOAT2(0.0f, 0.0f), Colors::White, 0.0f,
                               XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));
    uptrSpriteBatch->End();

    // IMGUI
    static int count = 0;
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Test");
    ImGui::DragFloat("alpha", &alpha, 0.02f, 0, 1);
    // ImGui::DragFloat3("Translation xyz", translationOffset, 0.01f);
    ImGui::SameLine();
    ImGui::Text(to_string(count).c_str());
    ImGui::End();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // SWAP CHAIN
    this->cptrSwapchain->Present(1, NULL); // 1 = vsync
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

    DXGI_SWAP_CHAIN_DESC scd = {0};

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
    COM_ERROR_IF_FAILED(hr, L"FAILED CREATE DEVICE AND SWAP CHAIN");

    ComPtr<ID3D11Texture2D> backBuffer;
    hr = this->cptrSwapchain->GetBuffer(
        0, __uuidof(ID3D11Texture2D),
        reinterpret_cast<void **>(backBuffer.GetAddressOf()));
    COM_ERROR_IF_FAILED(hr, L"GETBUFFER FAILED");

    // RENDER TARGET VIEW
    hr = this->cptrDevice->CreateRenderTargetView(
        backBuffer.Get(), NULL, this->cptrRenderTargetView.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, L"FAILED CERATE RENDER TARGET");

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
    COM_ERROR_IF_FAILED(hr, L"FAILED CERATE DEPTH STENCIL BUFFER");

    // CREATE DEPTH STENCIL VIEW
    hr = this->cptrDevice->CreateDepthStencilView(
        this->cptrDepthStencilBuffer.Get(), NULL,
        this->cptrDepthStencilView.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, L"FAILED CERATE DEPTH STENCIL VIEW");

    // SET OUTPUT MERGER
    this->cptrDeviceContext->OMSetRenderTargets(
        1, this->cptrRenderTargetView.GetAddressOf(),
        this->cptrDepthStencilView.Get());

    // CREATE DEPTH STENCIL STATE
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {0};

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask =
        D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc =
        D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    hr = this->cptrDevice->CreateDepthStencilState(
        &depthStencilDesc, this->cptrDepthStencilState.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, L"FAILED CERATE DEPTH STENCIL STATE");

    // CREATE VIEWPORT
    D3D11_VIEWPORT viewport = {0};

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
    ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
    rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    hr = this->cptrDevice->CreateRasterizerState(
        &rasterizerDesc, this->cptrRasterizerState.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, L"FAILED TO CREATE RASTERIZER STATE");

    // Create Blend State
    D3D11_BLEND_DESC blendDesc = {0};

    D3D11_RENDER_TARGET_BLEND_DESC rtbd = {0};

    rtbd.BlendEnable = true;
    rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
    rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
    rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
    rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
    rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
    rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
    rtbd.RenderTargetWriteMask =
        D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

    blendDesc.RenderTarget[0] = rtbd;

    hr = this->cptrDevice->CreateBlendState(
        &blendDesc, this->cptrBlendstate.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, L"FALED TO CREATE BLEND STATE");

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
    COM_ERROR_IF_FAILED(hr, L"Failed To Create Sampler State");

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
        Vertex(-0.5f, -0.5f, 0.5f, 0, 1), // F BOTTOM LEFT - [0]
        Vertex(-0.5f, 0.5f, 0.5f, 0, 0),  // F TOP LEFT - [1]
        Vertex(0.5f, 0.5f, 0.5f, 1, 0),   // F TOP RIGHT - [2]
        Vertex(0.5f, -0.5f, 0.5f, 1, 1),  // F BOTTOM RIGHT [3]
        Vertex(-0.5f, -0.5f, -0.5f, 0, 1), // B BOTTOM LEFT - [4]
        Vertex(-0.5f, 0.5f, -0.5f, 0, 0),  // B TOP LEFT - [5]
        Vertex(0.5f, 0.5f, -0.5f, 1, 0),   // B TOP RIGHT - [6]
        Vertex(0.5f, -0.5f, -0.5f, 1, 1),  // B BOTTOM RIGHT [7]
    };

    // Load Vertex Buffer
    HRESULT hr =
        this->vertexBuffer.Initialize(this->cptrDevice.Get(), v, ARRAYSIZE(v));
    COM_ERROR_IF_FAILED(hr, L"FAILED TO CREATE BUFFER");

    // Load Index Data
    DWORD indices[] = {
        0, 1, 2, // FRONT
        0, 2, 3, 
        4, 5, 1, // LEFT 
        4, 1, 0, 
        3, 2, 6, // RIGHT
        3, 6, 7,
        1, 5, 6, // TOP
        1, 6, 2, 
        0, 4, 7, // BOTTOM
        0, 7, 3, 
        4, 5, 6, // BACK
        4, 6, 7,
    };

    hr = this->indexBuffer.Initialize(this->cptrDevice.Get(), indices,
                                      ARRAYSIZE(indices));
    COM_ERROR_IF_FAILED(hr, L"FAILED TO INDICES BUFFER");

    // Load Texture
    hr = CreateWICTextureFromFile(this->cptrDevice.Get(),
                                  L"Data/Textures/Stone1.png", nullptr,
                                  this->cptrPinkTexture.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, L"FAILED TO CREATE WIC Texture");

    // Initialize Constant Buffer
    hr = this->cb_vs_vertexshaderBuffer.Initialize(this->cptrDevice.Get());
    COM_ERROR_IF_FAILED(hr, L"FAILED TO CREATE CONSTANT BUFFER");

    hr = this->cb_ps_pixelshaderBuffer.Initialize(this->cptrDevice.Get());
    COM_ERROR_IF_FAILED(hr, L"FAILED TO CREATE CONSTANT BUFFER");

    camera.SetPosition(0.0f, 0.0f, -2.0f);
    float aspectRadio =
        static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    camera.SetProjectionValues(90.0f, aspectRadio, 0.1f, 1000.0f);

    return true;
}