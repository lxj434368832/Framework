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
		LOGM("��ǰ�����·��Ϊ:%s", szFullPath);
		LOGM("%s", "�����ζ�ţ��ҹ�Ŀǰ�Ѿ����������͵���������������ٴ����顣\
			��������ϴ�Ⱦ������Ч�İ취�������ж����������������֣���ʵ������Ȼ�ĵֿ�����\
			�������¹ڷ��ײ�����SARS�����������͵Ĺ�״�����������ڵ�����ϵͳ�޷�ʶ��\
			�����ˡ��ϵ�Ϊ�ѡ������������������ã�����Ҫ�����������ϵͳ����ʶ�������������ʲô���ӣ�\
			Ȼ�󷢶�����ϵͳ��������");
	}

	DWORD dwCount1 = GetTickCount();
	std::cout << "�ܹ���ʱ��" << dwCount1 - dwCount << std::endl;
	return 0;
}
