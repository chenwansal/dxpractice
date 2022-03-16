#include "Graphics.h"

// public instance
bool Graphics::Initialize(HWND hwnd, int width, int height) {

    if (!InitializeDirectX(hwnd, width, height)) {
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
    this->ptrDeviceContext->ClearRenderTargetView(
        this->ptrRenderTargetView.Get(), bgcolor);

    // SET INPUT LAYOUT
    this->ptrDeviceContext->IASetInputLayout(
        this->vertexshader.GetInputLayout());

    // SET PRIMITIVE TOPOLOGY
    this->ptrDeviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // SET RASTERIZER STATE
    this->ptrDeviceContext->RSSetState(this->ptrRasterizerState.Get());

    // SET VERTEX SHADER
    this->ptrDeviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);

    // SET PIXEL SHADER
    this->ptrDeviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);

    // SET VERTEX BUFFERS
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    this->ptrDeviceContext->IASetVertexBuffers(
        0, 1, ptrVertexBuffer.GetAddressOf(), &stride, &offset);

    // DRAW
    this->ptrDeviceContext->Draw(3, 0);

    // SWAP CHAIN
    this->ptrSwapchain->Present(1, NULL);

}

// private instance
bool Graphics::InitializeDirectX(HWND hwnd, int width, int height) {

    // DEVICE AND SWAPCHAIN
    vector<GraphicAdapterData> adapters =
        GraphicAdapterReader::GetAdapterDatas();

    if (adapters.size() < 1) {
        PLogger::PopupError("NO IDXGI ADAPTER");
        return false;
    }

    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
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
        D3D11_SDK_VERSION, &scd, this->ptrSwapchain.GetAddressOf(),
        this->ptrDevice.GetAddressOf(), NULL,
        this->ptrDeviceContext.GetAddressOf());

    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr,
                                      "FAILED CREATE DEVICE AND SWAP CHAIN");
        return false;
    }

    ComPtr<ID3D11Texture2D> backBuffer;
    hr = this->ptrSwapchain->GetBuffer(
        0, __uuidof(ID3D11Texture2D),
        reinterpret_cast<void **>(backBuffer.GetAddressOf()));

    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "GETBUFFER FAILED");
        return false;
    }

    // RENDER TARGET VIEW
    hr = this->ptrDevice->CreateRenderTargetView(
        backBuffer.Get(), NULL, this->ptrRenderTargetView.GetAddressOf());

    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "CERATE RENDER TARGET");
        return false;
    }

    this->ptrDeviceContext->OMSetRenderTargets(
        1, this->ptrRenderTargetView.GetAddressOf(), NULL);

    // CREATE VIEWPORT
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)width;
    viewport.Height = (float)height;

    // Set Viewport
    this->ptrDeviceContext->RSSetViewports(1, &viewport);

    // Create Rasterizer State
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
    rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    hr = this->ptrDevice->CreateRasterizerState(
        &rasterizerDesc, this->ptrRasterizerState.GetAddressOf());
    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "FAILED TO CREATE RASTERIZER STATE");
        return false;
    }

    return true;
}

bool Graphics::InitializeShaders() {

    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0,
         D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
         D3D11_APPEND_ALIGNED_ELEMENT,
         D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}};
    UINT numElements = ARRAYSIZE(layoutDesc);

    wstring path = PathHelper::GetEnvironmentDir();
    if (!vertexshader.Initialize(ptrDevice, path + L"vertexshader.cso",
                                 layoutDesc, numElements)) {
        return false;
    }

    if (!pixelshader.Initialize(ptrDevice, path + L"pixelshader.cso",
                                layoutDesc, numElements)) {
        return false;
    }

    return true;
}

bool Graphics::InitializeScene() {

    XMFLOAT3 redColor = XMFLOAT3(1.0f, 0.0f, 0.0f);
    XMFLOAT3 greenColor = XMFLOAT3(0.0f, 1.0f, 0.0f);
    XMFLOAT3 blueColor = XMFLOAT3(0.0f, 0.0f, 1.0f);

    // Vertices
    Vertex v[] = {
        Vertex(0.0f, 0.5f, redColor), // TOP
        Vertex(0.5f, -0.5f, greenColor),  // RIGHT
        Vertex(-0.5f, -0.5f, blueColor), // LEFT
    };

    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v);
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData;
    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = v;

    HRESULT hr =
        this->ptrDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData,
                                      this->ptrVertexBuffer.GetAddressOf());
    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "FAILED TO CREATE BUFFER");
        return false;
    }

    return true;
}
