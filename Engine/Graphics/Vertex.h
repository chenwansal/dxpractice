#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct Vertex {

    XMFLOAT3 pos;
    XMFLOAT3 color;

    Vertex() {
        this->pos = XMFLOAT3();
        this->color = XMFLOAT3();
    }
    Vertex(XMFLOAT3 pos, XMFLOAT3 color){
        this->pos = pos;
        this->color = color;
    }
    Vertex(float x, float y, float z, XMFLOAT3 color) {
        this->pos = XMFLOAT3(x, y, z);
        this->color = color;
    }
    Vertex(float x, float y, float z, float r, float g, float b) {
        this->pos = XMFLOAT3(x, y, z);
        this->color = XMFLOAT3(r, g, b);
    }
};
