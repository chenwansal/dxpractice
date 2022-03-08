#pragma once
#include "ErrorLogger.h"

class RenderWindow {
  public:
    bool Initialize(HINSTANCE hInstance, string window_title,
                    string window_class, int width, int height);
    bool ProcessMessages();
    ~RenderWindow();

  private:
    void RegisterWindowClass();
    HWND handle = NULL;
    HINSTANCE hInstance = NULL;
    string window_title = "";
    wstring window_title_wide = L"";
    string window_class = "";
    wstring window_class_wide = L"";
    int width = 0;
    int height = 0;
};