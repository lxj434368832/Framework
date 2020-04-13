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
		LOGM("��ǰ�����·��Ϊ:%s", szFullPath);
	}
	DWORD dwCount1 = GetTickCount();
	logw() << "��һ������ʱ��" << dwCount1 - dwCount << "ms";

	dwCount = GetTickCount();
	for (int i = 0; i < 1000; i++)
	{
		logd() << "��ǰ�����·��Ϊ:" << szFullPath;
	}
	dwCount1 = GetTickCount();
	loge() << "�ڶ��ι�����ʱ:" << dwCount1 - dwCount << "ms";
	logd() << "�����˳���";
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
		LOGM("��ǰ�����·��Ϊ:%s", szFullPath);
	}
	DWORD dwCount1 = GetTickCount();
	LOGM("����ʱ��%dms", dwCount1 - dwCount);

	return 0;
}