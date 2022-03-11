#pragma once
#include "../Keyboard/KeyboardDevice.h"
#include "../Mouse/MouseDevice.h"
#include "./RenderWindow.h"
#include "IWindowContainer.h"

class WindowContainer : IWindowContainer {
  public:
    KeyboardDevice keyboard;
    MouseDevice mouse;

    bool Initialize(HINSTANCE hInstance, string window_title,
                    string window_class, int width, int height);
    bool Process();
    LRESULT CALLBACK OnWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

  private:
    RenderWindow renderWindow;
};