#pragma once

#include<chrono>
using namespace std::chrono;

class CElapsedTimer
{
public:
	CElapsedTimer()
	{
		//QueryPerformanceFrequency(&_frequency);
		update();
	}
	~CElapsedTimer()
	{}

	void  update()
	{
		//QueryPerformanceCounter(&_startCount);
		_begin = high_resolution_clock::now();
	}
	/**
	*   获取当前秒
	*/
	double getElapsedSecond()
	{
		return  getElapsedTimeInMicroSec() * 0.000001;
	}
	/**
	*   获取毫秒
	*/
	double getElapsedTimeInMilliSec()
	{
		return this->getElapsedTimeInMicroSec() * 0.001;
	}
	/**
	*   获取微妙
	*/
	long long getElapsedTimeInMicroSec()
	{
		/*
		LARGE_INTEGER endCount;
		QueryPerformanceCounter(&endCount);
		double  startTimeInMicroSec =   _startCount.QuadPart * (1000000.0 / _frequency.QuadPart);
		double  endTimeInMicroSec   =   endCount.QuadPart * (1000000.0 / _frequency.QuadPart);
		return  endTimeInMicroSec - startTimeInMicroSec;
		*/

		return duration_cast<microseconds>(high_resolution_clock::now() - _begin).count();
	}

protected:
	//LARGE_INTEGER   _frequency;
	//LARGE_INTEGER   _startCount;
	time_point<high_resolution_clock> _begin;
};

void TestCELLTimestamp()
{
	CElapsedTimer stamp;
	stamp.update();
	unsigned uExpect = 100;
	::Sleep(uExpect);
	long long llActElapsed = stamp.getElapsedTimeInMicroSec();
	uExpect *= 1000;
	uint64_t llSub = llActElapsed - uExpect;

	printf("expect interval %d us, actual interval:%I64d us, sub:%I64d us.\n", 
		uExpect, llActElapsed,llSub);
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

	ret = MySleep(test); // 调用此函数开始延时, 返回实际花销的数量;

	DWORD dwAct = (DWORD)(1000000 * ret.QuadPart / frequence.QuadPart); // 换算到微秒数;
	printf("实际花费了:%d\n", dwAct);
}
