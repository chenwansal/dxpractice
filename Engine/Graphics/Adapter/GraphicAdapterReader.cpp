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
    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr, "Failed to create DXGIFactory");
        exit(-1);
    }

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
    if (FAILED(hr)) {
        PLogger::PopupErrorWithResult(hr,
                                      "Failed to Get Desc for IDXGIAdapter");
    }
}