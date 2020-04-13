#pragma once

#include "..\..\include\CharacterSet.h"
#include "..\..\include\charset.h"
#include "..\..\include\LogFile.h"

int TestCharactorTransport()
{
	std::wstring strUnicode(L"w��������������");
	std::string strAnsi1("w��������������");
	std::string strAnsi = mqwUtils::UnicodeToANSI(strUnicode.data());
	std::string strUTF8 = mqwUtils::UnicodeToUTF8(strUnicode.data());
	std::string strAns3 = charset::UnicodeToANSI(strUnicode.data());

	for (int i = 0; i < 100; i++)
	{
		LOGM("ת��ǰ��Ansi�ַ���Ϊ:%s", strAnsi1.data());
		LOGW("ת�����Ansi�ַ���Ϊ:%s", strAnsi.data());
		Sleep(1000);
	}

	//logm() << "ת��ǰ��Ansi�ַ���Ϊ:" << strAnsi1;
	//logm() << "ת�����Ansi�ַ���Ϊ:" << strAnsi;
	system("pause");
}