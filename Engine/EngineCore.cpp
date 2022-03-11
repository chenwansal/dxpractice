#include "./EngineCore.h"

EngineCore::EngineCore() {
}

EngineCore::~EngineCore() {
}

bool EngineCore::Initialize(HINSTANCE hInstance, string titleName,
                            string className, int width, int height) {
    // WINDOW
    bool isInit = this->mainWindow.Initialize(hInstance, titleName, className,
                                              width, height);

    if (!isInit) {
        PLogger::PopupError("Init Failed");
    }

    return isInit;

}

bool EngineCore::Process() {
    return this->mainWindow.Process();
}

void EngineCore::Update() {
    this->mainWindow.Update();
}