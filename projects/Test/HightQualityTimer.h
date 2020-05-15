//////////////////////////////////////////////////////////////////////// ///////
// KTimer.h
//
//　　　　　 Windows Graphics Programming Win32 GDI and DirectDraw®
//　　　　　 Feng Yuan
//　　　　　 Publisher: Prentice Hall PTR
//　　　　　 First Edition December 01, 2000
//
//　 高精度纳秒计时器, 最后修改:
//　　　　　 2008-12　 by cheungmine@gmail.com
/////////////////////////////////////////////////////////////////////////////// 

#pragma once
#ifndef STRICT
#define STRICT
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

inline unsigned __int64 GetCycleCount(void)
{
	_asm	_emit	0x0F
	_asm	_emit	0x31
}

class KTimer
{
	unsigned __int64	m_startcycle;
public:
	unsigned __int64	m_overhead;	// Clock Cycles

	KTimer(void)
	{
		m_overhead = 0;
		Start();
		m_overhead = Stop();
	}

	// 启动CPU时钟
	void Start(void)
	{
		m_startcycle = GetCycleCount();
	}

	// 停止CPU时钟, 返回自上一次启动的时钟周期数
	unsigned __int64 Stop(void)
	{
		return GetCycleCount() - m_startcycle - m_overhead;
	}

	// 把以CPU周期数转为纳秒
	unsigned __int64 static CyclesToNanos(unsigned __int64 time_cycles, unsigned int speed_mhz)
	{
		return time_cycles * 1000 / speed_mhz;
	}

	// 把以CPU周期数转为毫秒
	unsigned __int64 static CyclesToMicros(unsigned __int64 time_cycles, unsigned int speed_mhz)
	{
		return time_cycles / speed_mhz;
	}

	// 把以CPU周期数转为毫秒
	unsigned __int64 static CyclesToMillis(unsigned __int64 time_cycles, unsigned int speed_mhz)
	{
		return time_cycles / speed_mhz / 1000;
	}

	// 1GHz = 1000MHz
	unsigned int CPUSpeedMHz()
	{
		Start();
		Sleep(1000);
		unsigned __int64 cputime = Stop();
		return (unsigned int)(cputime / 1000000);
	}
};


/* Usage:*/
#include <stdio.h>

int TestHightQualityTimer()
{
	KTimer	kt;
	unsigned int cpu_speed = kt.CPUSpeedMHz();
	kt.Start();
	Sleep(1000);
	unsigned int elapsed_cyc = (unsigned int)kt.Stop();
	printf("CPU Speed: %.2f Ghz. Elapsed %ld CPU Cycles:\n ( %ld Nanosecond, %ld um)\n",
		cpu_speed / 1000.f,
		elapsed_cyc,
		KTimer::CyclesToNanos(elapsed_cyc, cpu_speed));
	return 0;
}


// 功能：执行实际的延时功能, Interval 参数为需要执行的延时与时间有关的数量,

// 此函数返回执行后实际所用的时间有关的数量 ;

LARGE_INTEGER MySleep(LARGE_INTEGER Interval)
{
	LARGE_INTEGER privious, current, Elapse;

	QueryPerformanceCounter(&privious);

	current = privious;

	while (current.QuadPart - privious.QuadPart < Interval.QuadPart)
		QueryPerformanceCounter(&current);

	Elapse.QuadPart = current.QuadPart - privious.QuadPart;

	return Elapse;

}

void TestHightQualityTimer1()
{
	LARGE_INTEGER frequence;

	// 取高精度运行计数器的频率, 若硬件不支持则返回FALSE

	if (!QueryPerformanceFrequency(&frequence))

		printf("Your computer hardware doesn't support"

		" the high-resolution performance counter\n");

	LARGE_INTEGER test, ret;

	// 通过频率换算微秒数到对应的CPU时钟数量, 1秒 = 1000000微秒;
	DWORD dwm_dwTest = 1000;		//单位us
	test.QuadPart = frequence.QuadPart * dwm_dwTest / 1000000;

	ret = MySleep(test); // 调用此函数开始延时, 返回实际花销的数量 ;

	DWORD dwAct = (DWORD)(1000000 * ret.QuadPart / frequence.QuadPart); // 换算到微秒数;
	printf("实际花费了:%d\n", dwAct);
}