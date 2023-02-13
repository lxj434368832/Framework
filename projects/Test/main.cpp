#define _CRT_SECURE_NO_WARNINGS
//#include <vld.h>
#include <iostream>
#include <tchar.h>
#include "LogTest.h"
//#include "..\..\include\LogFile.h"
#include "HightQualityTimer.h"
#include "CElapsedTimer.h"
#include "LineArcInter.h"
#include "CharacterConvert.h"

#ifdef _DEBUG
#pragma comment(lib,"../../lib/Frameworkd.lib")
#else
#pragma comment(lib,"../../lib/Framework.lib")
#endif


int _tmain(int argc, _TCHAR* argv[])
{
	//LogFile log;
	//TestHightQualityTimer();
	//TestLineInter(); 
	//TestLineInter1();
	//TestLineInter2();
	//TestDebugLog();
	//system("pause");
	//DeleteLibLog();
	TestCharacterConvert();
	return 0;
}

