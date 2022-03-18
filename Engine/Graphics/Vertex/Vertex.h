#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct Vertex {

    XMFLOAT3 pos;
    // XMFLOAT3 color;
    XMFLOAT2 uv;

    Vertex() {
        this->pos = XMFLOAT3();
        this->uv = XMFLOAT2();
    }
    Vertex(XMFLOAT3 pos, XMFLOAT2 uv) {
        this->pos = pos;
        this->uv = uv;
    }
    Vertex(float x, float y, float z, XMFLOAT2 uv) {
        this->pos = XMFLOAT3(x, y, z);
        this->uv = uv;
    }
    Vertex(float x, float y, float z, float u, float v) {
        this->pos = XMFLOAT3(x, y, z);
        this->uv = XMFLOAT2(u, v);
    }
};
