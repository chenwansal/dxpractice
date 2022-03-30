#pragma once
#include "../Shader/ConstantBufferTypes.h"
#include "../ShaderBuffer/ConstantBuffer.h"
#include "../ShaderBuffer/IndexBuffer.h"
#include "../ShaderBuffer/VertexBuffer.h"
#include "../Vertex/Vertex.h"
#include "../Transform/Transform.h"

using namespace DirectX;

class Model {

  public:

    Transform transform;

    bool Initialize(ID3D11Device *ptrDevice,
                    ID3D11DeviceContext *ptrDeviceContext,
                    ID3D11ShaderResourceView *ptrTexture,
                    ConstantBuffer<CB_VS_vertexshader> &cb_vs_vertexshader);
    void SetTexture(ID3D11ShaderResourceView *ptrTexture);
    void Draw(const XMMATRIX &viewProjectionMatrix);

  private:
    ID3D11Device *ptrDevice = nullptr;
    ID3D11DeviceContext *ptrDeviceContext = nullptr;
    ConstantBuffer<CB_VS_vertexshader> *cb_vs_vertexshader = nullptr;
    ID3D11ShaderResourceView *texture = nullptr;

    VertexBuffer<Vertex> vertexBuffer;
    IndexBuffer indexBuffer;

    void UpdateWorldMatrix();
};