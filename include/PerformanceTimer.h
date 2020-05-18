#pragma once
/*************************************************************************
* function���߾����̶߳�ʱ��
* author :	������
* datetime��2020-05-16
* company:  ������
*************************************************************************/

#include <functional>
typedef std::function<void(unsigned)> PfTimerProc;

class PerformanceTimer
{
public:
	PerformanceTimer();
	~PerformanceTimer();

	/*************************************************************************
	* function��		����һ����ʱ��
	* param uInterval:	��ʱʱ�䣬��λ΢��us
	* param lpTimerFunc:�ص�����
	* return unsigned:	�ɹ�����ID,ʧ�ܷ���0
	*************************************************************************/
	unsigned AcquireTimer(unsigned uInterval, PfTimerProc lpTimerFunc);

	/*************************************************************************
	* function��		ֹͣǰ�洴���Ķ�ʱ��
	* param uTimerId:	��ʱ��Id
	* return void:		��
	*************************************************************************/
	void ReleaseTimer(unsigned uTimerId);

private:
	void TimerCallback();

private:
	struct SPerformanceTimerData	*d;
};

