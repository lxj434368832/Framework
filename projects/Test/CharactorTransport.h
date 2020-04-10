#pragma once

#include "..\..\include\CharacterSet.h"
#include "..\..\include\charset.h"
#include "..\..\include\LogFile.h"

int TestCharactorTransport()
{
	std::wstring strUnicode(L"w①我我我我我我");
	std::string strAnsi1("w①我我我我我我");
	std::string strAnsi = mqwUtils::UnicodeToANSI(strUnicode.data());
	std::string strUTF8 = mqwUtils::UnicodeToUTF8(strUnicode.data());
	std::string strAns3 = charset::UnicodeToANSI(strUnicode.data());

	for (int i = 0; i < 100; i++)
	{
		LOGM("转换前的Ansi字符串为:%s", strAnsi1.data());
		LOGW("转换后的Ansi字符串为:%s", strAnsi.data());
		Sleep(1000);
	}

	//logm() << "转换前的Ansi字符串为:" << strAnsi1;
	//logm() << "转换后的Ansi字符串为:" << strAnsi;
	system("pause");
}