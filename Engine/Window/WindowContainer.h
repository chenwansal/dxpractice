#pragma once
#include "../Keyboard/KeyboardDevice.h"
#include "./RenderWindow.h"
#include "IWindowContainer.h"

class WindowContainer : IWindowContainer {
  public:
    bool Initialize(HINSTANCE hInstance, string window_title,
                    string window_class, int width, int height);
    bool Process();

  private:
    RenderWindow renderWindow;
    KeyboardDevice keyboard;
};