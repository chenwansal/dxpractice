#pragma once
#include <queue>
#include "KeyboardEvent.h"
using namespace std;

class KeyboardDevice {
  public:
    KeyboardDevice();
    bool IsKeyBufferEmpty();
    bool IsKeyPressed(const unsigned char key);
    KeyboardEvent ReadKey();
    void RecordKeyPressed(const unsigned char key);
    void RecordKeyReleased(const unsigned char key);
    void RecordChar(const unsigned char key);
    
  private:
    bool keyStatus[256];
    queue<KeyboardEvent> keyEventBufferQueue;
    queue<unsigned char> keyCharBufferQueue;
};