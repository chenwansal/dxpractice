#pragma once
#include "../Base/StringConverter.h"
#include "../Exception/COMException.h"
#include <Windows.h>

class PLogger {
  public:
    static void ConsoleLog(string message);
    static void PopupError(string message);
    static void PopupErrorWithResult(HRESULT hr, string message);
    static void PopupErrorWithResult(HRESULT hr, wstring message);
    static void PopupException(COMException &ex);
};