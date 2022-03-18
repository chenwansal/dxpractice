#pragma once
#include "../../Logger/PLogger.h"
#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;
using namespace std;

class VertexShader {
  public:
    bool Initialize(ComPtr<ID3D11Device> &ptrDevice, wstring shaderPath,
                    D3D11_INPUT_ELEMENT_DESC *ptrDesc, UINT numElements);
    ID3D11VertexShader *GetShader();
    ID3D10Blob *GetBuffer();
    ID3D11InputLayout *GetInputLayout();

  private:
    ComPtr<ID3D11VertexShader> ptrShader;
    ComPtr<ID3D10Blob> ptrShaderBuffer;
    ComPtr<ID3D11InputLayout> ptrInputLayout;
};

class PixelShader {
  public:
    bool Initialize(ComPtr<ID3D11Device> &ptrDevice, wstring shaderPath,
                    D3D11_INPUT_ELEMENT_DESC *ptrDesc, UINT numElements);
    ID3D11PixelShader *GetShader();
    ID3D10Blob *GetBuffer();

  private:
    ComPtr<ID3D11PixelShader> ptrShader;
    ComPtr<ID3D10Blob> ptrShaderBuffer;
};