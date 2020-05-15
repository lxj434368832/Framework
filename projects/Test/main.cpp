#include <vld.h>
#include <iostream>
#include <tchar.h>
//#include "LogTest.h"
//#include "..\..\include\LogFile.h"
#include "HightQualityTimer.h"
#include "CELLTimestamp.h"

#ifdef _DEBUG
#pragma comment(lib,"../../lib/Frameworkd.lib")
#else
#pragma comment(lib,"../../lib/Framework.lib")
#endif


int _tmain(int argc, _TCHAR* argv[])
{
	//LogFile log;
	TestHightQualityTimer();
	system("pause");
	return 0;
}

