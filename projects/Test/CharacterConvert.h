#pragma once
#include "..\..\include\CharacterSet.h"
//#include "..\..\include\charset.h"
#include "..\..\include\LogFile.h"

int TestCharactorTransport()
{
	std::wstring strUnicode(L"w①我我我我我我");
	std::string strAnsi1("w①我我我我我我");
	std::string strAnsi = mqwUtils::UnicodeToANSI(strUnicode.data());
	std::string strUTF8 = mqwUtils::UnicodeToUTF8(strUnicode.data());
	//std::string strAns3 = charset::UnicodeToANSI(strUnicode.data());

	for (int i = 0; i < 100; i++)
	{
		LOGM("转换前的Ansi字符串为:%s", strAnsi1.data());
		LOGW("转换后的Ansi字符串为:%s", strAnsi.data());
		Sleep(1000);
	}

	//logm() << "转换前的Ansi字符串为:" << strAnsi1;
	//logm() << "转换后的Ansi字符串为:" << strAnsi;
	system("pause");
	return 0;
}

#include "../../include/CharacterCvt.h"
#include "../../include/Utils.h"
#include <fstream>
#include <iostream>

void TestCharacterConvert()
{
	void TestCharacterConvert1();
	return TestCharacterConvert1();

	std::wstring wstr(L"金山办公123");
	std::string str = mqwUtils::WideStringToUTF8String(wstr.c_str(), wstr.length());
	wstr = mqwUtils::UTF8StringToWideString(str.c_str(), str.length());

	//std::ifstream fin("/home/mingel/MyProgram/AdvanceProgram/Test1/TestText.txt");
	std::ifstream fin("F:\\ProgramSrc\\Framework\\projects\\Test\\TestText.txt");
	if (!fin)
		return;
	std::string strSrc;
	std::list<std::wstring> listWString;
	while (std::getline(fin, strSrc))
	{
		listWString.push_back(mqwUtils::MBStringToWideString(strSrc.c_str(), strSrc.length()));
	}
	fin.close();

	std::list<std::string> listString;
	MElapsedTimer elapse;
	for (auto& wStr : listWString)
		listString.push_back(mqwUtils::WideStringToUTF8String(wStr.c_str(), wStr.length()));
	uint64_t u64Elapse = elapse.ElapsedMicroseconds();

	std::cout << "wide string convert multi-byte string use " << u64Elapse << " us" << std::endl;
	return;
}

std::wstring s2ws(const std::string& s)
{
	std::wstring strWide;
	const char* lpLocale = setlocale(LC_ALL, "");
	size_t szSize = mbstowcs(nullptr, s.c_str(), 0);
	if (-1 == szSize) return strWide;
	strWide.resize(szSize);
	mbstowcs(&strWide[0], s.c_str(), szSize);
	setlocale(LC_ALL, lpLocale);
	return strWide;
}

std::string ws2s(const std::wstring& ws)
{
	std::string mb;
	const char* lpLocale = setlocale(LC_ALL, "");
	size_t szSize = wcstombs(nullptr, ws.c_str(), 0);
	if (-1 == szSize) return mb;
	mb.resize(szSize);
	wcstombs(&mb[0], ws.c_str(), szSize);
	setlocale(LC_ALL, lpLocale);
	return mb;
}

struct WstringVsString {
	std::wstring ws;
	std::string bs;
}const EncodeCases[] = {
	{L"x\u0080\u00ff\u0100\u208cy", u8"x\200ÿĀ₌y"},
	{L"金山办公", u8"金山办公"},
	{L"데이터 선언을 수정할 때 키워드는", u8"데이터 선언을 수정할 때 키워드는"},
	{L"データ宣言を修飾するとき", u8"データ宣言を修飾するとき"}
};

void TestCharacterConvert1()
{
	for (auto& value : EncodeCases)
	{
		std::string mb = mqwUtils::WideStringToUTF8String(value.ws.c_str(), value.ws.length());
		if (mb == value.bs)
		{
			printf("equal\n");
		}
		else {
			printf("not equal\n");
		}
	}
	/*std::string str = mqwUtils::WideStringToUTF8String(wstr.c_str(), wstr.length());
	std::wstring strWide1 = s2ws(str);*/
	return;
}
