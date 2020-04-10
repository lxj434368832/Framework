#include <vld.h>
#include <iostream>
#include <tchar.h>
#include "..\..\include\LogFile.h"
#include "LogTest.h"

#ifdef _DEBUG
#pragma comment(lib,"../../lib/Frameworkd.lib")
#else
#pragma comment(lib,"../../lib/Framework.lib")
#endif


int _tmain(int argc, _TCHAR* argv[])
{
	LogFile log;

	TestLibLog();
	system("pause");
	return 0;
}

