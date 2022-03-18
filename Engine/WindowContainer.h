#pragma once
#include "../Keyboard/KeyboardDevice.h"
#include "../Mouse/MouseDevice.h"
#include "./RenderWindow.h"
#include "IWindowContainer.h"
#include "../Graphics/Graphics.h"

class WindowContainer : IWindowContainer {
  public:
    bool Initialize(HINSTANCE hInstance,
                    string window_title, string window_class, int width,
                    int height);
    bool Process();
    void Update();
    void RenderFrame();
    LRESULT CALLBACK OnWindowProc(HWND hwnd, UINT msg, WPARAM wParam,
                                  LPARAM lParam);

  private:
    KeyboardDevice keyboard;
    MouseDevice mouse;
    Graphics gfx;
    RenderWindow renderWindow;
};