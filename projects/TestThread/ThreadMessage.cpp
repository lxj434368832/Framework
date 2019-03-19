#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <process.h>
#define UM_MSG1 WM_USER+1
#define UM_MSG2 WM_USER+2

using namespace std;

DWORD WINAPI Thread1(LPVOID para)
{

	DWORD dwThreadId = *(DWORD *)para;
	DWORD i = 0;
	TCHAR *p;
	char strTmp[100];

	while (TRUE)
	{
		Sleep(1700);
		p = new TCHAR[10];
		sprintf(strTmp, "Hello %d %x", i++, p);
		PostThreadMessage(dwThreadId, UM_MSG1, (WPARAM)strTmp, (LPARAM)p);
		//delete []p;
	}

	return 0;
}

DWORD WINAPI Thread2(LPVOID para)
{
	char strTmp[100];
	DWORD dwThreadId = *(DWORD *)para;
	DWORD i = 0;
	TCHAR *p;
	while (TRUE)
	{

		Sleep(3000);
		p = new TCHAR[10];
		sprintf(strTmp, "World %d %x", i++, p);
		PostThreadMessage(dwThreadId, UM_MSG2, (WPARAM)strTmp, (LPARAM)p);
		//delete []p;
	}

	return 0;

}

int main()
{
	DWORD dwValue = GetCurrentThreadId();
	cout << "GetCurrentThreadId " << dwValue << endl;;
	HANDLE hThread1 = CreateThread(NULL, 0, &Thread1, &dwValue, 0, NULL);
	HANDLE hThread2 = CreateThread(NULL, 0, &Thread2, &dwValue, 0, NULL);
	CloseHandle(hThread1);
	CloseHandle(hThread2);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		switch (msg.message)
		{
		case UM_MSG1:
		case UM_MSG2:
			printf("msg:0x%x /t%x /t%s0/n", msg.message, msg.lParam, msg.wParam);
			//注释掉这句你就会看到堆变化
			delete[](TCHAR *)msg.lParam;
			break;
		default:
			printf("Unknown msg:0x%x/n", msg.message);
			break;
		}
		Sleep(100);
	}
	return 0;
}