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
		LOGM("��ǰ�����·��Ϊ:%s", szFullPath);
	}
	DWORD dwCount1 = GetTickCount();
	logw() << "��һ������ʱ��" << dwCount1 - dwCount;

	dwCount = GetTickCount();
	for (int i = 0; i < 1000; i++)
	{
		logd()<<"��ǰ�����·��Ϊ:" << szFullPath;
	}
	dwCount1 = GetTickCount();
	loge() << "�ڶ��ι�����ʱ:" << dwCount1 - dwCount;
	logd() << "�����˳���";

	//system("pause");
	return 0;
}
