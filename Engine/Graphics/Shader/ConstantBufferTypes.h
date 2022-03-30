#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct CB_VS_vertexshader {
    XMMATRIX mat;
};

struct CB_PS_pixelshader {
    float alpha;
};