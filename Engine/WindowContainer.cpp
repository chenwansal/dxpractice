#include "WindowContainer.h"

WindowContainer::WindowContainer() {
    static bool raw_input_initialized = true;
    if (raw_input_initialized) {
        RAWINPUTDEVICE rid;
        rid.usUsagePage = 0x01; // MOUSE
        rid.usUsage = 0x02;
        rid.dwFlags = 0;
        rid.hwndTarget = 0;
        if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE) {
            PLogger::PopupErrorWithResult(
                GetLastError(), "Failed To Register raw ipnut devices");
            exit(-1);
        }
    }
}

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

    float dt = timer.GetDeltaTime();
    timer.Restart();

    while (!this->keyboard.IsKeyBufferEmpty()) {
        KeyboardEvent kbe = this->keyboard.ReadKey();
        unsigned char keycode = kbe.GetKeyCode();
    }

    const float CAMERA_ROT_SPEED = 1.0f;
    const float CAMERA_FWD_SPEED = 0.02f;
    const float CAMERA_MOVE_SPPED = 2.0f;
    while (!this->mouse.IsEventBufferEmpty()) {
        MouseEvent me = this->mouse.ReadEvent();
        // PLogger::ConsoleLog(me);
        if (mouse.IsRightDown()) {
            if (me.GetType() == MouseEvent::EventType::RAW_MOVE) {
                MousePoint delta = me.GetPos();
                this->gfx.camera.AdjustRotation(
                    -(float)delta.y * CAMERA_ROT_SPEED * dt,
                    -(float)delta.x * CAMERA_ROT_SPEED * dt, 0);
            }
        }
        if (me.GetType() == MouseEvent::EventType::WheelScrollUp) {
            this->gfx.camera.AdjustPosition(gfx.camera.GetForwardVector() *
                                            CAMERA_FWD_SPEED * me.GetPosY() *
                                            dt);
        } else if (me.GetType() == MouseEvent::EventType::WheelScrollDown) {
            this->gfx.camera.AdjustPosition(gfx.camera.GetForwardVector() *
                                            -CAMERA_FWD_SPEED * me.GetPosY() *
                                            dt);
        }
    }

    if (keyboard.IsKeyPressed('W')) {
        gfx.camera.AdjustPosition(0, -CAMERA_MOVE_SPPED * dt, 0);
    }
    if (keyboard.IsKeyPressed('S')) {
        gfx.camera.AdjustPosition(0, CAMERA_MOVE_SPPED * dt, 0);
    }
    if (keyboard.IsKeyPressed('A')) {
        gfx.camera.AdjustPosition(CAMERA_MOVE_SPPED * dt, 0, 0);
    }
    if (keyboard.IsKeyPressed('D')) {
        gfx.camera.AdjustPosition(-CAMERA_MOVE_SPPED * dt, 0, 0);
    }
}

void WindowContainer::RenderFrame() {
    this->gfx.RenderFrame();
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg,
                                              WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowContainer::OnWindowProc(HWND hwnd, UINT msg,
                                               WPARAM wParam, LPARAM lParam) {

    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) {
        return true;
    }

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
    case WM_INPUT: {
        // NEED TO UNDERSTAND THIS PART
        // NEED TO UNDERSTAND THIS PART
        // NEED TO UNDERSTAND THIS PART
        UINT dataSize = {0};
        GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL,
                        &dataSize, sizeof(RAWINPUTHEADER));
        if (dataSize > 0) {
            unique_ptr<BYTE[]> rawdata = make_unique<BYTE[]>(dataSize);
            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,
                                rawdata.get(), &dataSize,
                                sizeof(RAWINPUTHEADER)) == dataSize) {
                RAWINPUT *raw = reinterpret_cast<RAWINPUT *>(rawdata.get());
                if (raw->header.dwType == RIM_TYPEMOUSE) {
                    mouse.RecordMouseRawMove(raw->data.mouse.lLastX,
                                             raw->data.mouse.lLastY);
                }
            }
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}