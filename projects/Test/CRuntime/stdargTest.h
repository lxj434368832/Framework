#pragma once

#include <cstdarg>
#include <stdio.h>

int GetAverage(int v1, ...)
{
	int nAve = 0;
	int nCount = 0;
	va_list args;
	va_start(args, v1);
	do 
	{
		nAve += v1;
		++nCount;
		v1 = va_arg(args, int);
	} while (v1);

	nAve /= nCount;
}

void TestStdarg()
{
	int nAve = GetAverage(1, 2, 3, 0);
	printf_s("average is :%d \n", nAve);
}
