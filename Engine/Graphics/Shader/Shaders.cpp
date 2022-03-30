#include "Shaders.h"

// ==== VERTEX SHADER ====
bool VertexShader::Initialize(ComPtr<ID3D11Device> &ptrDevice,
                              wstring shaderPath,
                              D3D11_INPUT_ELEMENT_DESC *ptrDesc,
                              UINT numElements) {
    HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(),
                                   this->ptrShaderBuffer.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, L"Failed to load vertex shader: " + shaderPath);

    hr =
        ptrDevice->CreateVertexShader(this->ptrShaderBuffer->GetBufferPointer(),
                                      this->ptrShaderBuffer->GetBufferSize(),
                                      NULL, this->ptrShader.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, L"Failed to Create Vertex shader: " + shaderPath);

    hr = ptrDevice->CreateInputLayout(ptrDesc, numElements,
                                      this->ptrShaderBuffer->GetBufferPointer(),
                                      this->ptrShaderBuffer->GetBufferSize(),
                                      this->ptrInputLayout.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, L"INIT SHADERS FAILED ");

    return true;
}

ID3D11VertexShader *VertexShader::GetShader() {
    return this->ptrShader.Get();
}

ID3D10Blob *VertexShader::GetBuffer() {
    return this->ptrShaderBuffer.Get();
}

ID3D11InputLayout *VertexShader::GetInputLayout() {
    return this->ptrInputLayout.Get();
}

// ==== PIXEL SHADER ====

bool PixelShader::Initialize(ComPtr<ID3D11Device> &ptrDevice,
                             wstring shaderPath,
                             D3D11_INPUT_ELEMENT_DESC *ptrDesc,
                             UINT numElements) {
    HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(),
                                   this->ptrShaderBuffer.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, L"Failed to load pixel shader: " + shaderPath);

    hr = ptrDevice->CreatePixelShader(
        this->ptrShaderBuffer.Get()->GetBufferPointer(),
        this->ptrShaderBuffer.Get()->GetBufferSize(), NULL,
        this->ptrShader.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, L"Failed to Create Pixel shader: " + shaderPath);
    return true;
}

ID3D11PixelShader *PixelShader::GetShader() {
    return this->ptrShader.Get();
}

ID3D10Blob *PixelShader::GetBuffer() {
    return this->ptrShaderBuffer.Get();
}
