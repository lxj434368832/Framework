#pragma once

#include <string>
#include <windows.h>
#include <stdint.h>

namespace charset
{
	const std::string ans_to_utf8(const std::string &a_str); // ANSI?UTF-8(c++11)
	const std::string utf8_to_ans(const std::string &u_str); // UTF-8?ANSI(c++11)
	
	const std::string UnicodeToANSI(const std::wstring& src);
	const std::wstring ANSIToUnicode(const std::string& src);
	const std::string UnicodeToUTF8(const std::wstring& src);
	const std::wstring UTF8ToUnicode(const std::string& src);
}
