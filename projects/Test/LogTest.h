#pragma once

#include <windows.h>
#include <atlconv.h>
#include "..\..\include\LogFile.h"

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
}



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
	}
	DWORD dwCount1 = GetTickCount();
	LOGM("计用时：%dms", dwCount1 - dwCount);

	return 0;
}