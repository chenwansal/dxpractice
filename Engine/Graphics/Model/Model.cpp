#include "Model.h"

bool Model::Initialize(ID3D11Device *ptrDevice,
                       ID3D11DeviceContext *ptrDeviceContext,
                       ID3D11ShaderResourceView *ptrTexture,
                       ConstantBuffer<CB_VS_vertexshader> &cb_vs_vertexshader) {

    this->ptrDevice = ptrDevice;
    this->ptrDeviceContext = ptrDeviceContext;
    this->texture = ptrTexture;
    this->cb_vs_vertexshader = &cb_vs_vertexshader;

    // Vertices Square
    Vertex v[] = {
        Vertex(-0.5f, -0.5f, 0.5f, 0, 1),  // F BOTTOM LEFT - [0]
        Vertex(-0.5f, 0.5f, 0.5f, 0, 0),   // F TOP LEFT - [1]
        Vertex(0.5f, 0.5f, 0.5f, 1, 0),    // F TOP RIGHT - [2]
        Vertex(0.5f, -0.5f, 0.5f, 1, 1),   // F BOTTOM RIGHT [3]
        Vertex(-0.5f, -0.5f, -0.5f, 0, 1), // B BOTTOM LEFT - [4]
        Vertex(-0.5f, 0.5f, -0.5f, 0, 0),  // B TOP LEFT - [5]
        Vertex(0.5f, 0.5f, -0.5f, 1, 0),   // B TOP RIGHT - [6]
        Vertex(0.5f, -0.5f, -0.5f, 1, 1),  // B BOTTOM RIGHT [7]
    };

    // Load Vertex Buffer
    HRESULT hr =
        this->vertexBuffer.Initialize(this->ptrDevice, v, ARRAYSIZE(v));
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

    hr = this->indexBuffer.Initialize(this->ptrDevice, indices,
                                      ARRAYSIZE(indices));
    COM_ERROR_IF_FAILED(hr, L"FAILED TO INDICES BUFFER");

    this->UpdateWorldMatrix();
    return true;
}

void Model::SetTexture(ID3D11ShaderResourceView *ptrTexture) {
    this->texture = ptrTexture;
}

void Model::Draw(const XMMATRIX &viewProjectionMatrix) {
    cb_vs_vertexshader->data.mat =
        this->transform.GetWorldMatrix() * viewProjectionMatrix;
    cb_vs_vertexshader->data.mat =
        XMMatrixTranspose(cb_vs_vertexshader->data.mat);
    cb_vs_vertexshader->ApplyChanges(this->ptrDeviceContext);
    this->ptrDeviceContext->VSSetConstantBuffers(
        0, 1, cb_vs_vertexshader->GetAddressOf());

    UINT offset = 0;
    this->ptrDeviceContext->PSSetShaderResources(0, 1, &this->texture);
    this->ptrDeviceContext->IASetVertexBuffers(
        0, 1, this->vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(),
        &offset);
    this->ptrDeviceContext->IASetIndexBuffer(this->indexBuffer.Get(),
                                             DXGI_FORMAT_R32_UINT, 0);
    this->ptrDeviceContext->DrawIndexed(this->indexBuffer.BufferSize(), 0, 0);
}

void Model::UpdateWorldMatrix() {
    transform.UpdateWorldMatrix();
}
