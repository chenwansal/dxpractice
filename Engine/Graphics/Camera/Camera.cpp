#include "Camera.h"

Camera::Camera() {
    this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->posVector = XMLoadFloat3(&this->pos);
    this->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio,
                                 float nearZ, float farZ) {
    float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
    this->projectionMatrix =
        XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const XMMATRIX &Camera::GetViewMatrix() const {
    return this->viewMatrix;
}

const XMMATRIX &Camera::GetProjectionMatrix() const {
    return this->projectionMatrix;
}

const XMVECTOR &Camera::GetPositionVector() const {
    return this->posVector;
}

const XMVECTOR &Camera::GetRotationVector() const {
    return this->rotVector;
}

const XMFLOAT3 &Camera::GetPositionFloat3() const {
    return this->pos;
}

const XMFLOAT3 &Camera::GetRotationFloat3() const {
    return this->rot;
}

const XMVECTOR &Camera::GetUpVector() {
    return this->vec_up;
}

const XMVECTOR &Camera::GetForwardVector() {
    return this->vec_forward;
}

const XMVECTOR &Camera::GetBackwardVector() {
    return this->vec_backward;
}

const XMVECTOR &Camera::GetLeftVector() {
    return this->vec_left;
}

const XMVECTOR &Camera::GetRightVector() {
    return this->vec_right;
}

void Camera::SetPosition(const XMVECTOR &pos) {
    XMStoreFloat3(&this->pos, pos);
    this->posVector = pos;
    this->UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z) {
    this->pos = XMFLOAT3(x, y, z);
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR &offset) {
    this->posVector += offset;
    XMStoreFloat3(&this->pos, this->posVector);
    this->UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z) {
    this->pos.x += x;
    this->pos.y += y;
    this->pos.z += z;
    this->posVector = XMLoadFloat3(&this->pos);
    this->UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR &rot) {
    this->rotVector = rot;
    XMStoreFloat3(&this->rot, rot);
    this->UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z) {
    this->rot = XMFLOAT3(x, y, z);
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR &offset) {
    this->rotVector += offset;
    XMStoreFloat3(&this->rot, this->rotVector);
    this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z) {
    this->rot.x += x;
    this->rot.y += y;
    this->rot.z += z;
    this->rotVector = XMLoadFloat3(&this->rot);
    this->UpdateViewMatrix();
}

void Camera::SetLookAtPos(float x, float y, float z) {

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

void Camera::UpdateViewMatrix() {
    XMMATRIX camRotMatrix =
        XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
    XMVECTOR camTarget =
        XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotMatrix);
    camTarget += this->posVector;
    XMVECTOR upDir =
        XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotMatrix);
    this->viewMatrix = XMMatrixLookAtLH(this->posVector, camTarget, upDir);

    XMMATRIX vecRotMat = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
    this->vec_forward =
        XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotMat);
    this->vec_backward =
        XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotMat);
    this->vec_left =
        XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotMat);
    this->vec_right =
        XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotMat);
}
