#include "WindowContainer.h"

bool WindowContainer::Initialize(HINSTANCE hInstance, string window_title,
                                 string window_class, int width, int height) {
    return this->renderWindow.Initialize(this, hInstance, window_title,
                                         window_class, width, height);
}

bool WindowContainer::Process() { return this->renderWindow.ProcessMessages(); }