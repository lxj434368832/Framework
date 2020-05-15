#pragma once

#include<chrono>
using namespace std::chrono;

class CELLTimestamp
{
public:
	CELLTimestamp()
	{
		//QueryPerformanceFrequency(&_frequency);
		//QueryPerformanceCounter(&_startCount);
		update();
	}
	~CELLTimestamp()
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

#include <windows.h>
void TestCELLTimestamp()
{
	CELLTimestamp stamp;
	stamp.update();
	unsigned uExpect = 100;
	::Sleep(uExpect);
	long llActElapsed = stamp.getElapsedTimeInMicroSec();
	uExpect *= 1000;
	long llSub = llActElapsed - uExpect;

	printf("expect interval %d us, actual interval:%ld us, sub:%ld us.\n", 
		uExpect, llActElapsed,llSub);
}