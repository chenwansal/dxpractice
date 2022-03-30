#pragma once
#include "Graphics/Graphics.h"
#include "Keyboard/KeyboardDevice.h"
#include "Mouse/MouseDevice.h"
#include "Timer/Timer.h"
#include "Window/RenderWindow.h"
#include "WindowInterface/IWindowContainer.h"

class WindowContainer : IWindowContainer {
  public:
    WindowContainer();
    bool Initialize(HINSTANCE hInstance, string window_title,
                    string window_class, int width, int height);
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
    Timer timer;
};