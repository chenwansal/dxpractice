#ifndef ConstantBuffer_h__
#define ConstantBuffer_h__
#include "../../Logger/PLogger.h"
#include <d3d11.h>
#include <wrl/client.h>
using namespace std;
using namespace Microsoft::WRL;

template <class T> class ConstantBuffer {
  private:
    ConstantBuffer(const ConstantBuffer<T> &rhs);

  private:
    ComPtr<ID3D11Buffer> buffer;

  public:
    ConstantBuffer() {
    }

    T data;

    ID3D11Buffer *Get() const {
        return buffer.Get();
    }

    ID3D11Buffer *const *GetAddressOf() const {
        return buffer.GetAddressOf();
    }

    HRESULT Initialize(ID3D11Device *ptrDevice) {

        if (buffer != nullptr) {
            buffer.Reset();
        }

        D3D11_BUFFER_DESC desc;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - sizeof(T) % 16));
        desc.StructureByteStride = 0;

        HRESULT hr = ptrDevice->CreateBuffer(&desc, 0, buffer.GetAddressOf());
        return hr;
    }

    bool ApplyChanges(ID3D11DeviceContext *ptrDeviceContext) {

        D3D11_MAPPED_SUBRESOURCE mappedResource;
        HRESULT hr = ptrDeviceContext->Map(
            buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        COM_ERROR_IF_FAILED(hr, L"FAILED TO map CONSTANT BUFFER");
        CopyMemory(mappedResource.pData, &data, sizeof(T));
        ptrDeviceContext->Unmap(this->buffer.Get(), 0);
        return true;
    }
};

#endif // !define ConstantBuffer_h__
