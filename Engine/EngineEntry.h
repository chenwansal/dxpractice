#pragma once
#include "./Window/RenderWindow.h"
#include "./Window/WindowContainer.h"

class EngineEntry {
  public:
    EngineEntry();
    ~EngineEntry();
    void Initialize(HINSTANCE hInstance, string titleName, string className,
                    int width, int height);
    bool Process();

  private:
    WindowContainer mainWindow;
};