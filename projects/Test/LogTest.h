#pragma once

#include <windows.h>
#include <atlconv.h>

/*#include "..\..\include\LogFile.h"

int TestAppLog()
{
	char szFullPath[MAX_PATH];
	::GetModuleFileNameA(nullptr, szFullPath, MAX_PATH);
	DWORD dwCount = GetTickCount();
	for (int i = 0; i < 1000; i++)
	{
		LOGM("当前程序的路径为:%s", szFullPath);
	}
	DWORD dwCount1 = GetTickCount();
	logw() << "第一共计用时：" << dwCount1 - dwCount << "ms";

	dwCount = GetTickCount();
	for (int i = 0; i < 1000; i++)
	{
		logd() << "当前程序的路径为:" << szFullPath;
	}
	dwCount1 = GetTickCount();
	loge() << "第二次共计用时:" << dwCount1 - dwCount << "ms";
	logd() << "程序将退出！";
	return 0;
}*/



#include "..\..\include\LibLog.h"

#define LIB_NAME "LogTest"

int TestLibLog()
{
	char szFullPath[MAX_PATH];
	::GetModuleFileNameA(nullptr, szFullPath, MAX_PATH);
	DWORD dwCount = GetTickCount();
	for (int i = 0; i < 1000; i++)
	{
		LOGM("当前程序的路径为:%s", szFullPath);
		LOGM("%s", "这就意味着，我国目前已经有两种类型的三款疫苗进入了临床试验。\
			疫苗是阻断传染病最有效的办法，人体中对于外来病毒的入侵，其实具有天然的抵抗力，\
			但是像新冠肺炎病毒、SARS病毒这类新型的冠状病毒，人体内的免疫系统无法识别，\
			发生了“认敌为友”的情况。而疫苗的作用，就是要让人体的免疫系统来认识病毒这个敌人是什么样子，\
			然后发动免疫系统攻击它。");
	}

	DWORD dwCount1 = GetTickCount();
	std::cout << "总共用时：" << dwCount1 - dwCount << std::endl;
	return 0;
}
