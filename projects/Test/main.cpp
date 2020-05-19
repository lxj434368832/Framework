#include <vld.h>
#include <iostream>
#include <tchar.h>
#include "LogTest.h"
//#include "..\..\include\LogFile.h"
#include "HightQualityTimer.h"
#include "CELLTimestamp.h"
#include "LineArcInter.h"

#ifdef _DEBUG
#pragma comment(lib,"../../lib/Frameworkd.lib")
#else
#pragma comment(lib,"../../lib/Framework.lib")
#endif


int _tmain(int argc, _TCHAR* argv[])
{
	//LogFile log;
	//TestHightQualityTimer();
	TestLineInter(); 
	TestLineInter1();
	//TestLineInter2();
	system("pause");
	DeleteLibLog();
	return 0;
}

