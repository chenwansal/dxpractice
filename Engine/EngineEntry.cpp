#include "./EngineEntry.h"

EngineEntry::EngineEntry() {
}

EngineEntry::~EngineEntry() {
}

void EngineEntry::Initialize(HINSTANCE hInstance, string titleName,
                             string className, int width, int height) {
    bool isInit = this->mainWindow.Initialize(hInstance, titleName, className,
                                              width, height);
    if (!isInit) {
        PLogger::PopupError("Init Failed");
    }
}

bool EngineEntry::Process() {
    return this->mainWindow.Process();
}

void EngineEntry::Update() {
    while (!this->mainWindow.keyboard.IsKeyBufferEmpty()) {
        KeyboardEvent kbe = this->mainWindow.keyboard.ReadKey();
        //PLogger::ConsoleLog("" + kbe.GetKeyCode());
    }

    while (!this->mainWindow.mouse.IsEventBufferEmpty()) {
        MouseEvent me = this->mainWindow.mouse.ReadEvent();
        //PLogger::ConsoleLog("dd");
    }
}
