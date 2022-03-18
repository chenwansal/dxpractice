#pragma once
#include "../../Logger/PLogger.h"
#include <d3d11.h>
#include <vector>
#include <wrl/client.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXGI.lib")
using Microsoft::WRL::ComPtr;
using namespace std;

// 显示设备
class GraphicAdapterData {
  public:
    GraphicAdapterData(IDXGIAdapter *ptrAdapter);
    IDXGIAdapter *ptrAdapter = nullptr;
    DXGI_ADAPTER_DESC desc;
};

class GraphicAdapterReader {
  public:
    static vector<GraphicAdapterData> GetAdapterDatas();

  private:
    static vector<GraphicAdapterData> adapterDatas;
};