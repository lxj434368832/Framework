#include "LogTest.h"
#include "..\..\include\LogFile.h"

void LogTest::TestLogFile()
{
	for (unsigned i = 0; i < 5000; i++)
	{
		logm() << "�߳� 'Win32 �߳�' (0x2af8) ���˳�������ֵΪ 0 (0x0)����־д���˳���:" << i;
	}
}
