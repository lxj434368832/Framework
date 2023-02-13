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
	//�����̳߳�
	PTP_POOL threadPool = CreateThreadpool(NULL);
	SetThreadpoolThreadMinimum(threadPool, 1);
	SetThreadpoolThreadMaximum(threadPool, 3);

	//���ι����ύ�����첽�ķ�ʽ���к�����һ��������
	TrySubmitThreadpoolCallback(poolThreadFunc, NULL, nullptr); 
	TrySubmitThreadpoolCallback(poolThreadFunc, NULL, nullptr);
	TrySubmitThreadpoolCallback(poolThreadFunc, NULL, nullptr);
	TrySubmitThreadpoolCallback(poolThreadFunc, NULL, nullptr);

	system("pause");
	//�ر��̳߳�
	CloseThreadpool(threadPool);
	//SuspendThread();   //�����߳�״̬Ϊ����
	//ResumeThread();    //�ָ��߳�״̬����
}

void TestThreadpoolWork()
{
	//�����̳߳�
	PTP_POOL threadPool = CreateThreadpool(NULL);
	SetThreadpoolThreadMinimum(threadPool, 1);
	SetThreadpoolThreadMaximum(threadPool, 3);
	//��ʼ������
	TP_CALLBACK_ENVIRON te;
	InitializeThreadpoolEnvironment(&te);
	SetThreadpoolCallbackPool(&te, threadPool);

	/*
	����������
	*/
	PTP_WORK pwk;
	pwk = CreateThreadpoolWork(poolThreadWork, NULL, &te);
	////�ύ����������ύ���
	SubmitThreadpoolWork(pwk);
	SubmitThreadpoolWork(pwk);
	SubmitThreadpoolWork(pwk);
	SubmitThreadpoolWork(pwk);
	SubmitThreadpoolWork(pwk);
	SubmitThreadpoolWork(pwk);

	system("pause");
	////�ȴ���������
	WaitForThreadpoolWorkCallbacks(pwk, false);
	//�رչ�������
	CloseThreadpoolWork(pwk);

	//�����̳߳صĻ�������
	DestroyThreadpoolEnvironment(&te);

}
