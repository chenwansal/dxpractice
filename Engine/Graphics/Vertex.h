#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct Vertex {

    XMFLOAT2 pos;
    XMFLOAT3 color;

    Vertex() : Vertex(0, 0, 1.0f, 1.0f, 1.0f) {
    }
    Vertex(float x, float y, XMFLOAT3 color) : Vertex(x, y, color.x, color.y, color.z) {
    }
    Vertex(float x, float y, float r, float g, float b) {
        this->pos.x = x;
        this->pos.y = y;
        this->color = XMFLOAT3(r, g, b);
    }
};
