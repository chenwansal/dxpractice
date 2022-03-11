#include "WindowContainer.h"

bool WindowContainer::Initialize(HINSTANCE hInstance, string window_title,
                                 string window_class, int width, int height) {
    this->keyboard = KeyboardDevice();
    this->mouse = MouseDevice();
    return this->renderWindow.Initialize(this, hInstance, window_title,
                                         window_class, width, height);
}

bool WindowContainer::Process() {
    return this->renderWindow.ProcessMessages();
}

LRESULT CALLBACK WindowContainer::OnWindowProc(HWND hwnd, UINT msg,
                                               WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    // KEYBOARD MESSAGE
    case WM_KEYDOWN: {
        unsigned char key = static_cast<unsigned char>(wParam);
        this->keyboard.RecordKeyPressed(key);
        return 0;
    }
    case WM_KEYUP: {
        unsigned char key = static_cast<unsigned char>(wParam);
        this->keyboard.RecordKeyReleased(key);
        return 0;
    }
    case WM_CHAR: {
        unsigned char key = static_cast<unsigned char>(wParam);
        this->keyboard.RecordChar(key);
        return 0;
    }

    // MOUSE MESSAGE
    case WM_MOUSEMOVE: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        this->mouse.RecordMouseMove(x, y);
        return 0;
    }
    case WM_LBUTTONDOWN: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        this->mouse.RecordLeftPressed(x, y);
        return 0;
    }
    case WM_LBUTTONUP: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        this->mouse.RecordLeftReleased(x, y);
        return 0;
    }
    case WM_RBUTTONDOWN: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        this->mouse.RecordRightPressed(x, y);
        return 0;
    }
    case WM_RBUTTONUP: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        this->mouse.RecordRightReleased(x, y);
        return 0;
    }
    case WM_MBUTTONDOWN: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        this->mouse.RecordMiddlePressed(x, y);
        return 0;
    }
    case WM_MBUTTONUP: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        this->mouse.RecordMiddleReleased(x, y);
        return 0;
    }
    case WM_MOUSEWHEEL: {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        short offset = GET_WHEEL_DELTA_WPARAM(wParam);
        if (offset > 0) {
            this->mouse.RecordWheelScrollUp(x, y);
        } else if (offset < 0) {
            this->mouse.RecordWheelScrollDown(x, y);
        }
        break;
    }
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}