#pragma once
#include "../Base/StringConverter.h"
#include <Windows.h>

class PLogger {
  public:
    static void ConsoleLog(string message);
    static void PopupError(string message);
    static void PopupErrorWithResult(HRESULT hr, string message);
};