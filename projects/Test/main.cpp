#include <vld.h>
#include "..\..\include\LogFile.h"
#include "..\..\include\CharacterSet.h"
#include "..\..\include\charset.h"
#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <atlconv.h>

#ifdef _DEBUG
#pragma comment(lib,"../../lib/Frameworkd.lib")
#else
#pragma comment(lib,"../../lib/Framework.lib")
#endif


int _tmain(int argc, _TCHAR* argv[])
{
	LogFile log;
	//char *strPath;
	//_get_pgmptr(&strPath);
	//std::cout << strPath << std::endl;

	//char szFullPath[MAX_PATH];
	//::GetModuleFileNameA(nullptr, szFullPath, MAX_PATH);
	//DWORD dwCount = GetTickCount();
	//for (int i = 0; i < 1000; i++)
	//{
	//LOGM("��ǰ�����·��Ϊ:%s", szFullPath);
	//}
	//DWORD dwCount1 = GetTickCount();
	//logw() << "��һ������ʱ��" << dwCount1 - dwCount;

	//dwCount = GetTickCount();
	//for (int i = 0; i < 1000; i++)
	//{
	//	logd()<<"��ǰ�����·��Ϊ:" << szFullPath;
	//}
	//dwCount1 = GetTickCount();
	//loge() << "�ڶ��ι�����ʱ:" << dwCount1 - dwCount;
	//logd() << "�����˳���";
	std::wstring strUnicode(L"w��������������");
	std::string strAnsi1("w��������������");
	std::string strAnsi = mqwUtils::UnicodeToANSI(strUnicode.data());
	std::string strUTF8 = mqwUtils::UnicodeToUTF8(strUnicode.data());
	std::string strAns3 = charset::UnicodeToANSI(strUnicode.data());

	logm() << "ת��ǰ��Ansi�ַ���Ϊ:" << strAnsi1;
	logm() << "ת�����Ansi�ַ���Ϊ:" << strAnsi;
	//system("pause");
	return 0;
}

