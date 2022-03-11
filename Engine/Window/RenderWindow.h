#pragma once
#include "../Logger/PLogger.h"
#include "IWindowContainer.h"

class RenderWindow {
  public:
    // MODEL
    HINSTANCE hInstance = NULL;
    HWND handle = NULL;

    // 构造
    RenderWindow();
    ~RenderWindow();

    bool Initialize(IWindowContainer *ptrWindowContainer, HINSTANCE hInstance,
                    string window_title, string window_class, int width,
                    int height);
    bool ProcessMessages();

  private:
    // MODEL
    string window_title = "";
    wstring window_title_wide = L"";
    string window_class = "";
    wstring window_class_wide = L"";
    int width = 0;
    int height = 0;

    void RegisterWindowClass();
    static LRESULT CALLBACK OnWindowProc(HWND hwnd, UINT msg, WPARAM wParam,
                                         LPARAM lParam);
    static LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT msg, WPARAM wParam,
                                       LPARAM lParam);
};