#include "./EngineEntry.h"

EngineEntry::EngineEntry() {}

EngineEntry::~EngineEntry() {}

void EngineEntry::Initialize(HINSTANCE hInstance, string titleName,
                             string className, int width, int height) {
    bool isInit = this->mainWindow.Initialize(hInstance, titleName, className,
                                              width, height);
    if (!isInit) {
        ErrorLogger::Log("Init Failed");
    }
}

bool EngineEntry::Process() { return this->mainWindow.Process(); }
