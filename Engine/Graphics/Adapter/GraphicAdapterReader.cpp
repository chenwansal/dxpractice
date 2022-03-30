#include "GraphicAdapterReader.h"

vector<GraphicAdapterData> GraphicAdapterReader::adapterDatas;

vector<GraphicAdapterData> GraphicAdapterReader::GetAdapterDatas() {
    if (adapterDatas.size() > 0) {
        return adapterDatas;
    }

    ComPtr<IDXGIFactory> ptrFactory;

    // Create a DXGIFactory Obj
    HRESULT hr =
        CreateDXGIFactory(__uuidof(IDXGIFactory),
                          reinterpret_cast<void **>(ptrFactory.GetAddressOf()));
    COM_ERROR_IF_FAILED(hr, L"Failed to create DXGIFactory");

    IDXGIAdapter *ptrAdapter;
    UINT index = 0;
    while (SUCCEEDED(ptrFactory->EnumAdapters(index, &ptrAdapter))) {
        adapterDatas.push_back(GraphicAdapterData(ptrAdapter));
        index += 1;
    }
    return adapterDatas;
}

GraphicAdapterData::GraphicAdapterData(IDXGIAdapter *ptrAdapter) {
    this->ptrAdapter = ptrAdapter;
    HRESULT hr = ptrAdapter->GetDesc(&this->desc);
    COM_ERROR_IF_FAILED(hr, L"Failed to Get Desc for IDXGIAdapter");
}