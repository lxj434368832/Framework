#include "LogTest.h"
#include "..\..\include\LogFile.h"

void LogTest::TestLogFile()
{
	for (unsigned i = 0; i < 5000; i++)
	{
		logm() << "线程 'Win32 线程' (0x2af8) 已退出，返回值为 0 (0x0)。日志写入的顺序号:" << i;
	}
}
