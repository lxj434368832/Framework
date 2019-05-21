#include <vld.h>
#include <stdlib.h>
#include <windows.h>
#include "..\..\include\Log\LogFile.h"

#ifdef _DEBUG
#pragma comment(lib,"../../lib/Log/Debug/Log.lib")
#else
#pragma comment(lib,"../../lib/Log/Release/Log.lib")
#endif // _DEBUG


int main()
{
	LogFile log;
	//char *strPath;
	//_get_pgmptr(&strPath);
	//std::cout << strPath << std::endl;

	char szFullPath[MAX_PATH];
	::GetModuleFileNameA(nullptr, szFullPath, MAX_PATH);
	DWORD dwCount = GetTickCount();
	for (int i = 0; i < 1000; i++)
	{
		LOGM("当前程序的路径为:%s", szFullPath);
	}
	DWORD dwCount1 = GetTickCount();
	logw() << "第一共计用时：" << dwCount1 - dwCount;

	dwCount = GetTickCount();
	for (int i = 0; i < 1000; i++)
	{
		logd()<<"当前程序的路径为:" << szFullPath;
	}
	dwCount1 = GetTickCount();
	loge() << "第二次共计用时:" << dwCount1 - dwCount;
	logd() << "程序将退出！";

	//system("pause");
	return 0;
}
