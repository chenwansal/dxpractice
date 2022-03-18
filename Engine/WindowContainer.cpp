#include "WindowContainer.h"

bool WindowContainer::Initialize(HINSTANCE hInstance, string window_title,
                                 string window_class, int width, int height) {
    // TIMER
    timer.Start();

    // KEYBOARD
    keyboard = KeyboardDevice();
    PLogger::ConsoleLog("keyboard init\r\n");

    // MOUSE
    mouse = MouseDevice();
    PLogger::ConsoleLog("mouse init\r\n");

    // WINDOW
    if (!renderWindow.Initialize(this, hInstance, window_title, window_class,
                                 width, height)) {
        return false;
    }
    PLogger::ConsoleLog("window init\r\n");

    // GRAPHIC
    if (!gfx.Initialize(renderWindow.GetHWND(), width, height)) {
        return false;
    }
    PLogger::ConsoleLog("graphic init\r\n");

    return true;
}

bool WindowContainer::Process() {
    return this->renderWindow.ProcessMessages();
}

void WindowContainer::Update() {

    float dt = timer.GetMillisecondsElapsed();
    timer.Restart();

    while (!this->keyboard.IsKeyBufferEmpty()) {
        KeyboardEvent kbe = this->keyboard.ReadKey();
        // PLogger::ConsoleLog(kbe);
    }

    while (!this->mouse.IsEventBufferEmpty()) {
        MouseEvent me = this->mouse.ReadEvent();
        // PLogger::ConsoleLog(me);
    }
}

void WindowContainer::RenderFrame() {
    this->gfx.RenderFrame();
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
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}