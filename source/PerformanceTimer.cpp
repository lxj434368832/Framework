#include "../include/CommonDefine.h"
#include "../include/PerformanceTimer.h"
#include "../include/LibLog.h"

#include <thread>
#include<chrono>
#include <map>
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace std::chrono;

struct SUserData
{
	unsigned long long	ullInterval;	//定时间隔
	PfTimerProc			fnTimer;		//定时器回调函数
	unsigned			uExtCount;		//执行计数
	time_point<high_resolution_clock> _begin;	//定时器开始时间点
	std::vector<unsigned long long> vctInterval; //间隔向量
	SUserData(unsigned uVal, PfTimerProc fn)
	{
		ullInterval = uVal;
		fnTimer = fn;
		uExtCount = 1;
		_begin = high_resolution_clock::now();
	}
};

struct SPerformanceTimerData
{
	bool							bStart = true;
	std::thread						*pTimerThread;	//定时器线程
	unsigned						uBaseId = rand() % 1000;
	std::map<unsigned, SUserData>	mapUsers;
	std::mutex						mtUsers;
	std::condition_variable			cvUsers;
};



PerformanceTimer::PerformanceTimer()
	:d(new SPerformanceTimerData)
{
	d->pTimerThread = new std::thread(&PerformanceTimer::TimerCallback, this);
}

PerformanceTimer::~PerformanceTimer()
{
	d->bStart = false;
	d->cvUsers.notify_all();
	if (d->pTimerThread->joinable()) d->pTimerThread->join();
	delete d->pTimerThread, d->pTimerThread = nullptr;

	delete d, d = nullptr;
}

unsigned PerformanceTimer::AcquireTimer(unsigned uInterval, PfTimerProc lpTimerFunc)
{
	d->mtUsers.lock();
	unsigned uTimerId = d->uBaseId++;
	d->mapUsers.emplace(uTimerId, SUserData(uInterval, lpTimerFunc));
	d->mtUsers.unlock();
	d->cvUsers.notify_one();

	//mingel test
	return uTimerId;
}

void PerformanceTimer::ReleaseTimer(unsigned uTimerId)
{
	std::vector<unsigned long long> vctInterval;

	d->mtUsers.lock();
	auto it = d->mapUsers.find(uTimerId);
	if (it == d->mapUsers.end())
	{
		d->mtUsers.unlock();
		LOGE("不存在当前定时器ID: %d ,请检查！", uTimerId);
	}
	else
	{
		vctInterval = it->second.vctInterval;
		d->mapUsers.erase(it);
		d->mtUsers.unlock();
	}

	//mingel test
	double dCalculate = 0;
	int iCount = vctInterval.size();
	for (int i = 1; i < iCount; i++)
	{
		unsigned long long ullInterval = vctInterval[i] - vctInterval[i - 1];
		LOGM("第%d次的间隔时间为：%u", i, ullInterval);
		dCalculate += ullInterval;
	}
	if (iCount) LOGM("本次定时间隔的平均值为：%f", dCalculate / iCount);
}

void PerformanceTimer::TimerCallback()
{
	LOGM("定时器线程:%d 开始。" ,std::this_thread::get_id());
	while (d->bStart)
	{
		std::unique_lock<std::mutex> lck(d->mtUsers);
		while (d->mapUsers.empty() && d->bStart)
		{
			d->cvUsers.wait(lck);
		}
		if (false == d->bStart) break;
		for (auto it = d->mapUsers.begin(); it != d->mapUsers.end(); it++)
		{
			SUserData &dt = it->second;
			unsigned long long ullCount = duration_cast<microseconds>(high_resolution_clock::now() - dt._begin).count();
			if (ullCount >= dt.ullInterval * dt.uExtCount)
			{
				dt.fnTimer(it->first);	//回调时间函数
				++dt.uExtCount;

				//mingel test
				dt.vctInterval.push_back(ullCount);
			}
		}
	}

	LOGM("定时器线程:%d 退出！", std::this_thread::get_id());
}
