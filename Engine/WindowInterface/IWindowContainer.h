#pragma once
#include <Windows.h>
#include <string>
using namespace std;

class IWindowContainer {
  public:
    virtual bool Initialize(HINSTANCE hInstance, string window_title,
                            string window_class, int width, int height) = 0;
    virtual LRESULT CALLBACK OnWindowProc(HWND hwnd, UINT msg, WPARAM wParam,
                                          LPARAM lParam) = 0;
};