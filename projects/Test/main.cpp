#include <vld.h>
#include "..\..\include\CharacterSet.h"
#include "LogTest.h"
#include "..\..\include\LogFile.h"
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
	LogTest test;
	test.TestLogFile();
	//char *strPath;
	//_get_pgmptr(&strPath);
	//std::cout << strPath << std::endl;

	//char szFullPath[MAX_PATH];
	//::GetModuleFileNameA(nullptr, szFullPath, MAX_PATH);
	//DWORD dwCount = GetTickCount();
	//for (int i = 0; i < 1000; i++)
	//{
	//LOGM("当前程序的路径为:%s", szFullPath);
	//}
	//DWORD dwCount1 = GetTickCount();
	//logw() << "第一共计用时：" << dwCount1 - dwCount;

	//dwCount = GetTickCount();
	//for (int i = 0; i < 1000; i++)
	//{
	//	logd()<<"当前程序的路径为:" << szFullPath;
	//}
	//dwCount1 = GetTickCount();
	//loge() << "第二次共计用时:" << dwCount1 - dwCount;
	//logd() << "程序将退出！";
	/*std::wstring strUnicode(L"w①我我我我我我");
	std::string strAnsi1("w①我我我我我我");
	std::string strAnsi = mqwUtils::UnicodeToANSI(strUnicode.data());
	std::string strUTF8 = mqwUtils::UnicodeToUTF8(strUnicode.data());

	for (int i = 0; i < 100; i++)
	{
		LOGM("转换前的Ansi字符串为:%s", strAnsi1.data());
		LOGW("转换后的Ansi字符串为:%s", strAnsi.data());
		Sleep(1000);
	}*/

	system("pause");
	return 0;
}

