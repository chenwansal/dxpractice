#pragma once
#include "Window/RenderWindow.h"
#include "WindowContainer.h"
#include "Graphics/Graphics.h"
#include "Keyboard/KeyboardDevice.h"
#include "Mouse/MouseDevice.h"

class EngineCore {
  public:
    EngineCore();
    ~EngineCore();
    bool Initialize(HINSTANCE hInstance, string titleName, string className,
                    int width, int height);
    bool Process();
    void Update();
    void RenderFrame();

  private:
    WindowContainer mainWindow;
};