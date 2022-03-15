#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct Vertex {

    XMFLOAT2 pos;

    Vertex():Vertex(0, 0) {
    }
    Vertex(float x, float y) {
        this->pos.x = x;
        this->pos.y = y;
    }
};
