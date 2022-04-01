#ifndef VertexBuffer_h__
#define VertexBuffer_h__
#include <d3d11.h>
#include <wrl/client.h>

using namespace std;
using namespace Microsoft::WRL;

template <class T> class VertexBuffer {
  private:
    VertexBuffer(const VertexBuffer<T> &rhs);

  private:
    ComPtr<ID3D11Buffer> buffer;
    UINT stride = sizeof(T);
    UINT vertexCount = 0;

  public:
    VertexBuffer() {
    }
    ID3D11Buffer *Get() const {
        return buffer.Get();
    }
    ID3D11Buffer *const *GetAddressOf() const {
        return buffer.GetAddressOf();
    }
    UINT VertexCount() const {
        return vertexCount;
    }

    const UINT Stride() const {
        return this->stride;
    }

    const UINT *StridePtr() const {
        return &this->stride;
    }

    HRESULT Initialize(ID3D11Device *device, T *data, UINT vertexCount) {

        if (buffer != nullptr) {
            buffer.Reset();
        }

        this->vertexCount = vertexCount;
        if (stride.get() == nullptr) {
            this->stride = make_unique<UINT>(sizeof(T));
        }

        D3D11_BUFFER_DESC vertexBufferDesc = {0};
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = stride * vertexCount;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
        vertexBufferData.pSysMem = data;

        HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData,
                                          this->buffer.GetAddressOf());
        return hr;
    }
};
#endif