#include "./RenderWindow.h"

RenderWindow::RenderWindow() {
}

RenderWindow::~RenderWindow() {
    if (this->handle != NULL) {
        UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
        DestroyWindow(handle);
    }
}

bool RenderWindow::Initialize(IWindowContainer *ptrWindowContainer,
                              HINSTANCE hInstance, string window_title,
                              string window_class, int width, int height) {
    this->hInstance = hInstance;
    this->width = width;
    this->height = height;
    this->window_title = window_title;
    this->window_title_wide = StringConverter::StringToWide(this->window_title);
    this->window_class = window_class;
    this->window_class_wide = StringConverter::StringToWide(this->window_class);

    this->RegisterWindowClass();

    this->handle = CreateWindowEx(
        0, this->window_class_wide.c_str(), this->window_title_wide.c_str(),
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 0, 0, this->width,
        this->height, NULL, NULL, this->hInstance, ptrWindowContainer);

    if (this->handle == NULL) {
        PLogger::PopupErrorWithResult(
            GetLastError(), "CreateWindowEx Failed: " + this->window_title);
        return false;
    }

    ShowWindow(this->handle, SW_SHOW);
    SetForegroundWindow(this->handle);
    SetFocus(this->handle);

    return true;
}

bool RenderWindow::ProcessMessages() {

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    if (PeekMessage(&msg, this->handle, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msg.message == WM_NULL) {
        if (!IsWindow(this->handle)) {
            this->handle = NULL;
            UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
            return false;
        }
    }

    return true;
}

void RenderWindow::RegisterWindowClass() {
    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = RenderWindow::OnWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = this->hInstance;
    wc.hIcon = NULL;
    wc.hIconSm = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = this->window_class_wide.c_str();
    wc.cbSize = sizeof(WNDCLASSEX);
    RegisterClassEx(&wc);
}

LRESULT CALLBACK RenderWindow::HandleMsgRedirect(HWND hwnd, UINT msg,
                                                 WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE: {
        DestroyWindow(hwnd);
        return 0;
    }
    default:
        IWindowContainer *ptrWindow = reinterpret_cast<IWindowContainer *>(
            GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (ptrWindow == nullptr) {
            PLogger::PopupError("null ptr window");
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        return ptrWindow->OnWindowProc(hwnd, msg, wParam, lParam);
    }
}

LRESULT CALLBACK RenderWindow::OnWindowProc(HWND hwnd, UINT msg, WPARAM wParam,
                                            LPARAM lParam) {
    switch (msg) {
    // 窗口创建回调
    case WM_NCCREATE: {
        const CREATESTRUCTW *const ptrCreate =
            reinterpret_cast<CREATESTRUCTW *>(lParam);
        IWindowContainer *ptrWindow =
            reinterpret_cast<IWindowContainer *>(ptrCreate->lpCreateParams);
        if (ptrWindow == nullptr) {
            PLogger::PopupError("null ptr window");
            exit(-1);
        }
        SetWindowLongPtr(hwnd, GWLP_USERDATA,
                         reinterpret_cast<LONG_PTR>(ptrWindow));
        SetWindowLongPtr(
            hwnd, GWLP_WNDPROC,
            reinterpret_cast<LONG_PTR>(RenderWindow::HandleMsgRedirect));
        return ptrWindow->OnWindowProc(hwnd, msg, wParam, lParam);
    }
    // 默认处理
    default: {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    }
}
