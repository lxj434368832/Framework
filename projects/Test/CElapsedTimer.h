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
	*   ��ȡ��ǰ��
	*/
	double getElapsedSecond()
	{
		return  getElapsedTimeInMicroSec() * 0.000001;
	}
	/**
	*   ��ȡ����
	*/
	double getElapsedTimeInMilliSec()
	{
		return this->getElapsedTimeInMicroSec() * 0.001;
	}
	/**
	*   ��ȡ΢��
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


// ���ܣ�ִ��ʵ�ʵ���ʱ����, Interval ����Ϊ��Ҫִ�е���ʱ��ʱ���йص�����,

// �˺�������ִ�к�ʵ�����õ�ʱ���йص����� ;

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

	// ȡ�߾������м�������Ƶ��, ��Ӳ����֧���򷵻�FALSE

	if (!QueryPerformanceFrequency(&frequence))

		printf("Your computer hardware doesn't support"

			" the high-resolution performance counter\n");

	LARGE_INTEGER test, ret;

	// ͨ��Ƶ�ʻ���΢��������Ӧ��CPUʱ������, 1�� = 1000000΢��;
	DWORD dwm_dwTest = 1000;		//��λus
	test.QuadPart = frequence.QuadPart * dwm_dwTest / 1000000;

	ret = MySleep(test); // ���ô˺�����ʼ��ʱ, ����ʵ�ʻ���������;

	DWORD dwAct = (DWORD)(1000000 * ret.QuadPart / frequence.QuadPart); // ���㵽΢����;
	printf("ʵ�ʻ�����:%d\n", dwAct);
}
