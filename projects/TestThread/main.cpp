#define  _CRT_SECURE_NO_WARNINGS 
#include "iostream"
#include "windows.h"
using namespace std;

void NTAPI poolThreadFunc(
	_Inout_ PTP_CALLBACK_INSTANCE Instance,
	_Inout_opt_ PVOID Context)
{
	cout << __FUNCTION__ << __LINE__ << " " << GetCurrentThreadId() << endl;
}

void NTAPI poolThreadWork(
	_Inout_ PTP_CALLBACK_INSTANCE Instance,
	_Inout_opt_ PVOID Context,
	_Inout_ PTP_WORK Work)
{
	cout<<__FUNCTION__<<__LINE__ <<" "<< GetCurrentThreadId() << endl;
}

void TestThreadpool();
void TestThreadpoolWork();

int main()
{
	TestThreadpoolWork();
	return 0;
}

void TestThreadpool()
{
	//创建线程池
	PTP_POOL threadPool = CreateThreadpool(NULL);
	SetThreadpoolThreadMinimum(threadPool, 1);
	SetThreadpoolThreadMaximum(threadPool, 3);

	//单次工作提交，以异步的方式运行函数，一次性任务
	TrySubmitThreadpoolCallback(poolThreadFunc, NULL, nullptr); 
	TrySubmitThreadpoolCallback(poolThreadFunc, NULL, nullptr);
	TrySubmitThreadpoolCallback(poolThreadFunc, NULL, nullptr);
	TrySubmitThreadpoolCallback(poolThreadFunc, NULL, nullptr);

	system("pause");
	//关闭线程池
	CloseThreadpool(threadPool);
	//SuspendThread();   //更改线程状态为悬挂
	//ResumeThread();    //恢复线程状态运行
}

void TestThreadpoolWork()
{
	//创建线程池
	PTP_POOL threadPool = CreateThreadpool(NULL);
	SetThreadpoolThreadMinimum(threadPool, 1);
	SetThreadpoolThreadMaximum(threadPool, 3);
	//初始化环境
	TP_CALLBACK_ENVIRON te;
	InitializeThreadpoolEnvironment(&te);
	SetThreadpoolCallbackPool(&te, threadPool);

	/*
	创建工作项
	*/
	PTP_WORK pwk;
	pwk = CreateThreadpoolWork(poolThreadWork, NULL, &te);
	////提交工作项，可以提交多次
	SubmitThreadpoolWork(pwk);
	SubmitThreadpoolWork(pwk);
	SubmitThreadpoolWork(pwk);
	SubmitThreadpoolWork(pwk);
	SubmitThreadpoolWork(pwk);
	SubmitThreadpoolWork(pwk);

	system("pause");
	////等待工作结束
	WaitForThreadpoolWorkCallbacks(pwk, false);
	//关闭工作对象
	CloseThreadpoolWork(pwk);

	//清理线程池的环境变量
	DestroyThreadpoolEnvironment(&te);

}
