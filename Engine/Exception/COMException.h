#pragma once
#include "../Base/StringConverter.h"
#include <comdef.h>
using namespace std;

#define COM_ERROR_IF_FAILED(hr, msg)                                           \
    if (FAILED(hr))                                                            \
        throw COMException(hr, msg, __FILE__, __FUNCTION__, __LINE__);

class COMException {
  public:
    COMException(HRESULT hr, const wstring &msg, const string &file,
                 const string &function, const int line) {
        _com_error error(hr);
        whatmsg =
            L"Msg: " + msg + L"\r\n";
        whatmsg += error.ErrorMessage();
        whatmsg += L"\r\nFile: " + StringConverter::StringToWide(string(file)) +
                   L"\r\n";
        whatmsg += L"Function: " +
                   StringConverter::StringToWide(string(function)) + L"\r\n";
        whatmsg += L"Line: " + StringConverter::StringToWide(to_string(line));
    }

    const wchar_t *what() const {
        return whatmsg.c_str();
    }

  private:
    wstring whatmsg;
};