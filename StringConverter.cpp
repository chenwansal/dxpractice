#include "StringConverter.h"
using namespace std;

wstring StringConverter::StringToWide(string str) {
    wstring wide_string(str.begin(), str.end());
    return wide_string;
}
