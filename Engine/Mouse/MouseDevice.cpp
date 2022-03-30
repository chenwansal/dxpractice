#include "MouseDevice.h"

// API
bool MouseDevice::IsLeftDown() {
    return this->isLeftDown;
}

bool MouseDevice::IsRightDown() {
    return this->isRightDown;
}

bool MouseDevice::IsMiddleDown() {
    return this->isMiddleButtonDown;
}

int MouseDevice::GetPosX() {
    return this->x;
}

int MouseDevice::GetPosY() {
    return this->y;
}

MousePoint MouseDevice::GetPos() {
    return {this->x, this->y};
}

bool MouseDevice::IsEventBufferEmpty() {
    return this->eventBufferQueue.empty();
}

MouseEvent MouseDevice::ReadEvent() {
    if (this->eventBufferQueue.empty()) {
        return MouseEvent();
    } else {
        MouseEvent me = this->eventBufferQueue.front();
        this->eventBufferQueue.pop();
        return me;
    }
}

// LEFT
void MouseDevice::RecordLeftPressed(int x, int y) {
    this->isLeftDown = true;
    MouseEvent me(MouseEvent::EventType::LPress, x, y);
    this->eventBufferQueue.push(me);
}

void MouseDevice::RecordLeftReleased(int x, int y) {
    this->isLeftDown = false;
    MouseEvent me(MouseEvent::EventType::LRelease, x, y);
    this->eventBufferQueue.push(me);
}

// RIGHT
void MouseDevice::RecordRightPressed(int x, int y) {
    this->isRightDown = true;
    MouseEvent me(MouseEvent::EventType::RPress, x, y);
    this->eventBufferQueue.push(me);
}

void MouseDevice::RecordRightReleased(int x, int y) {
    this->isRightDown = false;
    MouseEvent me(MouseEvent::EventType::RRelease, x, y);
    this->eventBufferQueue.push(me);
}

// MIDDLE
void MouseDevice::RecordMiddlePressed(int x, int y) {
    this->isMiddleButtonDown = true;
    MouseEvent me(MouseEvent::EventType::MPress, x, y);
    this->eventBufferQueue.push(me);
}

void MouseDevice::RecordMiddleReleased(int x, int y) {
    this->isMiddleButtonDown = false;
    MouseEvent me(MouseEvent::EventType::MRelease, x, y);
    this->eventBufferQueue.push(me);
}

// WHEEL
void MouseDevice::RecordWheelScrollDown(int x, int y) {
    MouseEvent me(MouseEvent::EventType::WheelScrollDown, x, y);
    this->eventBufferQueue.push(me);
}

void MouseDevice::RecordWheelScrollUp(int x, int y) {
    MouseEvent me(MouseEvent::EventType::WheelScrollUp, x, y);
    this->eventBufferQueue.push(me);
}

// MOVE
void MouseDevice::RecordMouseMove(int x, int y) {
    this->x = x;
    this->y = y;
    MouseEvent me(MouseEvent::EventType::Move, x, y);
    this->eventBufferQueue.push(me);
}

void MouseDevice::RecordMouseRawMove(int x, int y) {
    this->eventBufferQueue.push(
        MouseEvent(MouseEvent::EventType::RAW_MOVE, x, y));
}