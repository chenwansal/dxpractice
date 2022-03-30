#include "Transform.h"

void Transform::Reset() {
    this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->posVector = XMLoadFloat3(&this->pos);
    this->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->rotVector = XMLoadFloat3(&this->rot);
}

const XMVECTOR &Transform::GetPositionVector() const {
    return this->posVector;
}

const XMVECTOR &Transform::GetRotationVector() const {
    return this->rotVector;
}

const XMFLOAT3 &Transform::GetPositionFloat3() const {
    return this->pos;
}

const XMFLOAT3 &Transform::GetRotationFloat3() const {
    return this->rot;
}

const XMVECTOR &Transform::GetUpVector() {
    return this->vec_up;
}

const XMVECTOR &Transform::GetForwardVector() {
    return this->vec_forward;
}

const XMVECTOR &Transform::GetBackwardVector() {
    return this->vec_backward;
}

const XMVECTOR &Transform::GetLeftVector() {
    return this->vec_left;
}

const XMVECTOR &Transform::GetRightVector() {
    return this->vec_right;
}

void Transform::SetPosition(const XMVECTOR &pos) {
    XMStoreFloat3(&this->pos, pos);
    this->posVector = pos;
    this->UpdateWorldMatrix();
}

void Transform::SetPosition(const XMFLOAT3 &pos) {
    this->pos = pos;
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateWorldMatrix();
}

void Transform::SetPosition(float x, float y, float z) {
    this->pos = XMFLOAT3(x, y, z);
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateWorldMatrix();
}

void Transform::AdjustPosition(const XMVECTOR &offset) {
    this->posVector += offset;
    XMStoreFloat3(&this->pos, this->posVector);
    this->UpdateWorldMatrix();
}

void Transform::AdjustPosition(const XMFLOAT3 &offset) {
    this->pos.x += offset.x;
    this->pos.y += offset.y;
    this->pos.z += offset.z;
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateWorldMatrix();
}

void Transform::AdjustPosition(float x, float y, float z) {
    this->pos.x += x;
    this->pos.y += y;
    this->pos.z += z;
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateWorldMatrix();
}

void Transform::SetRotation(const XMVECTOR &rot) {
    this->rotVector = rot;
    XMStoreFloat3(&this->rot, rot);
    this->UpdateWorldMatrix();
}

void Transform::SetRotation(const XMFLOAT3 &rot) {
    this->rot = rot;
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateWorldMatrix();
}

void Transform::SetRotation(float x, float y, float z) {
    this->rot = XMFLOAT3(x, y, z);
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateWorldMatrix();
}

void Transform::AdjustRotation(const XMVECTOR &offset) {
    this->rotVector += offset;
    XMStoreFloat3(&this->rot, this->rotVector);
    this->UpdateWorldMatrix();
}

void Transform::AdjustRotation(const XMFLOAT3 &offset) {
    this->rot.x += offset.x;
    this->rot.y += offset.y;
    this->rot.z += offset.z;
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateWorldMatrix();
}

void Transform::AdjustRotation(float x, float y, float z) {
    this->rot.x += x;
    this->rot.y += y;
    this->rot.z += z;
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateWorldMatrix();
}

void Transform::SetLookAtPos(float x, float y, float z) {

    if (pos.x == x && pos.y == y && pos.z == z) {
        return;
    }

    x = this->pos.x - x;
    y = this->pos.y - y;
    z = this->pos.z - z;

    float pitch = 0.0f;
    if (y != 0.0f) {
        double v = (double)x * x + (double)z * z;
        const float distance = sqrt(v);
        pitch = atan(y / distance);
    }

    float yaw = 0.0f;
    if (x != 0.0f) {
        yaw = atan(x / z);
    }
    if (z > 0) {
        yaw += XM_PI;
    }

    this->SetRotation(pitch, yaw, 0.0f);
}

XMMATRIX Transform::GetWorldMatrix() {
    return this->worldMatrix;
}

void Transform::UpdateWorldMatrix() {
    this->worldMatrix =
        XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) *
        XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);

    XMMATRIX vecRotationMat = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
    this->vec_forward =
        XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMat);
    this->vec_backward = 
        XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMat);
    this->vec_left =
        XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMat);
    this->vec_right =
        XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMat);
}
