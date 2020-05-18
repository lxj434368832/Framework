#pragma once
/*************************************************************************
* function：高精度线程定时器
* author :	明巧文
* datetime：2020-05-16
* company:  明巧文
*************************************************************************/

#include <functional>
typedef std::function<void(unsigned)> PfTimerProc;

class PerformanceTimer
{
public:
	PerformanceTimer();
	~PerformanceTimer();

	/*************************************************************************
	* function：		开启一个定时器
	* param uInterval:	定时时间，单位微秒us
	* param lpTimerFunc:回调函数
	* return unsigned:	成功返回ID,失败返回0
	*************************************************************************/
	unsigned AcquireTimer(unsigned uInterval, PfTimerProc lpTimerFunc);

	/*************************************************************************
	* function：		停止前面创建的定时器
	* param uTimerId:	定时器Id
	* return void:		无
	*************************************************************************/
	void ReleaseTimer(unsigned uTimerId);

private:
	void TimerCallback();

private:
	struct SPerformanceTimerData	*d;
};

