#include "KeyboardDevice.h"

KeyboardDevice::KeyboardDevice() {
    for (int i = 0; i < 256; i += 1) {
        this->keyStatus[i] = false;
    }
}

bool KeyboardDevice::IsKeyBufferEmpty() {
    return this->keyEventBufferQueue.empty();
}

bool KeyboardDevice::IsKeyPressed(const unsigned char key) {
    return this->keyStatus[key];
}

KeyboardEvent KeyboardDevice::ReadKey() {
    if (this->keyEventBufferQueue.empty()) {
        return KeyboardEvent();
    } else {
        KeyboardEvent kbe = this->keyEventBufferQueue.front();
        this->keyEventBufferQueue.pop();
        return kbe;
    }
}

void KeyboardDevice::RecordKeyPressed(const unsigned char key) {
    this->keyStatus[key] = true;
    this->keyEventBufferQueue.push(
        KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void KeyboardDevice::RecordKeyReleased(const unsigned char key) {
    this->keyStatus[key] = false;
    this->keyEventBufferQueue.push(
        KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void KeyboardDevice::RecordChar(const unsigned char key) {
    this->keyCharBufferQueue.push(key);
}
