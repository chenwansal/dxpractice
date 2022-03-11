#pragma once
#include "MouseEvent.h"
#include <queue>
using namespace std;

class MouseDevice {
  public:
    // API
    bool IsLeftDown();
    bool IsMiddleDown();
    bool IsRightDown();

    int GetPosX();
    int GetPosY();
    MousePoint GetPos();

    bool IsEventBufferEmpty();
    MouseEvent ReadEvent();
    
    // LEFT
    void RecordLeftPressed(int x, int y);
    void RecordLeftReleased(int x, int y);
    // RIGHT
    void RecordRightPressed(int x, int y);
    void RecordRightReleased(int x, int y);
    // MIDDLE
    void RecordMiddlePressed(int x, int y);
    void RecordMiddleReleased(int x, int y);
    // WHEEL
    void RecordWheelScrollUp(int x, int y);
    void RecordWheelScrollDown(int x, int y);
    // MOVE
    void RecordMouseMove(int x, int y);

  private:
    queue<MouseEvent> eventBufferQueue;
    bool isLeftDown = false;
    bool isRightDown = false;
    bool isMiddleButtonDown = false;
    int x = 0;
    int y = 0;
};