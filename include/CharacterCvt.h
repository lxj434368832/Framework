#pragma once
#include <xstring>
#include <wtypes.h>
#include "FrameworkDefine.h"

BEGIN_NAMESPACE //namespace is mqwUtils

std::string WideStringToMBString(LPCWSTR lpwStr, size_t szLen = -1);
std::string WideStringToUTF8String(LPCWSTR lpwStr, size_t szLen = -1);
std::wstring MBStringToWideString(LPCSTR lpStr, size_t szLen = -1);
std::wstring UTF8StringToWideString(LPCSTR lpStr, size_t szLen = -1);

#ifdef _WIN32
std::string WinAnsiToUTF8(LPCSTR lpStr, size_t szLen = -1);
std::string UTF8ToWinAnsi(LPCSTR lpStr, size_t szLen = -1);
#endif

END_NAMESPACE