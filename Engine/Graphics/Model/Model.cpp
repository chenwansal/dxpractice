#include "Model.h"
#include "../../FBXImpot/ofbximporter.h"

bool Model::Initialize(const char *filePath, ID3D11Device *ptrDevice,
                       ID3D11DeviceContext *ptrDeviceContext,
                       ID3D11ShaderResourceView *ptrTexture,
                       ConstantBuffer<CB_VS_vertexshader> &cb_vs_vertexshader) {

    this->ptrDevice = ptrDevice;
    this->ptrDeviceContext = ptrDeviceContext;
    this->texture = ptrTexture;
    this->cb_vs_vertexshader = &cb_vs_vertexshader;

    vector<Vertex> tarVertices;
    vector<DWORD> tarIndices;

    ofbx::IScene *scene = load_fbx(filePath);
    int meshCount = scene->getMeshCount();
    for (int i = 0; i < meshCount; i += 1) {

        const ofbx::Mesh &mesh = *scene->getMesh(i);
        const ofbx::Geometry &geom = *mesh.getGeometry();

        // VERTEX
        int vertexCount = geom.getVertexCount();
        const ofbx::Vec3 *vertices = geom.getVertices();
        const ofbx::Vec2 *uvs = geom.getUVs();
        tarVertices.reserve(vertexCount);
        for (int j = 0; j < vertexCount; j += 1) {
            ofbx::Vec3 v = vertices[j];
            ofbx::Vec2 uv = uvs[j];
            tarVertices.push_back(Vertex(XMFLOAT3(v.x, v.y, v.z), XMFLOAT2(uv.x, uv.y)));
        }

        // INDEX
        int indexCount = geom.getIndexCount();
        const int *indices = geom.getFaceIndices();
        tarIndices.reserve(indexCount);
        for (int j = 0; j < indexCount; j += 1) {
            int index = indices[j];
            tarIndices.push_back(index);
        }

        // UV

        // NORMAL

        // COLOR
    }

    // Load Vertex Buffer
    HRESULT hr = this->vertexBuffer.Initialize(
        this->ptrDevice, tarVertices.data(), tarVertices.size());
    COM_ERROR_IF_FAILED(hr, L"FAILED TO CREATE BUFFER");

    // Load Index Buffer
    hr = this->indexBuffer.Initialize(this->ptrDevice, tarIndices.data(),
                                      tarIndices.size());
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
    if (this->texture != nullptr) {
        this->ptrDeviceContext->PSSetShaderResources(0, 1, &this->texture);
    }
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
