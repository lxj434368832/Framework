#include "../include/MTimer.h"
#include <iostream>
#include <minwindef.h>

std::map<UINT, funTimerProc> MTimer::s_mapUser;

std::recursive_mutex MTimer::s_mtxMap;

MTimer::MTimer()
{
}

MTimer::~MTimer()
{
}

DWORD MTimer::GetTimerId()
{
	if (m_uTimerID)
	{
		return m_uTimerID;
	}
	else
		return (DWORD)m_hThread;
}

bool MTimer::start(UINT uElapse, funTimerProc lpTimerFunc,bool isStartCall)
{
	if (m_hThread)
	{
		std::cout << "已经开启了一个定时器，id：" << m_uTimerID << "先前的定时器将停止" << std::endl;
		m_bStart = false;
		if (WAIT_TIMEOUT == ::WaitForSingleObject(m_hThread, 1000))
		{
			::CloseHandle(m_hThread);
		}
	}
	m_uElapse = uElapse;
	m_funTimerProc = lpTimerFunc;
	m_bIsStartCall = isStartCall;

	m_hThread = ::CreateThread(NULL, 0, onThreadProc, this, 0, NULL);
	if (NULL == m_hThread)
	{
		std::cout << "创建线程失败，错误码：" << ::GetLastError() << std::endl;
		return false;
	}
	return true;
}

DWORD WINAPI MTimer::onThreadProc(LPVOID lpParameter)
{

	MTimer *pThis = (MTimer*)lpParameter;
	return pThis->ThreadProc();
}

DWORD MTimer::ThreadProc()
{
	if (m_bIsStartCall)
	{
		m_funTimerProc((DWORD)m_hThread);
	}

	while (m_bStart)
	{
		::Sleep(m_uElapse);
		m_funTimerProc((DWORD)m_hThread);
	}

	return 0;
}

void MTimer::stop()
{
	if (m_hThread)
	{
		m_bStart = false;
		if (WAIT_TIMEOUT == ::WaitForSingleObject(m_hThread, 500))
		{
			::CloseHandle(m_hThread);
		}
		m_hThread = NULL;
	}
	else
		std::cout << "timer has stoped!" << std::endl;
}


bool MTimer::start_timer(UINT uElapse, funTimerProc lpTimerFunc)
{
	if (m_hThread)
	{
		std::cout << "已经开启了一个定时器，id：" << m_uTimerID<<"先前的定时器将停止" << std::endl;
		m_bStart = false;
		if (WAIT_TIMEOUT == ::WaitForSingleObject(m_hThread, 1000))
		{
			::CloseHandle(m_hThread);
		}
	}

	m_uElapse = uElapse;
	m_funTimerProc1 = lpTimerFunc;
	m_hThread = ::CreateThread(NULL, 0, onThreadProc1, this, 0, NULL);
	if (NULL == m_hThread)
	{
		std::cout << "创建线程失败，错误码：" << ::GetLastError() << std::endl;
		return false;
	}
	return true;
}

DWORD WINAPI MTimer::onThreadProc1(LPVOID lpParameter)
{
	MTimer *pThis = (MTimer*)lpParameter;
	return pThis->ThreadProc1();
}

DWORD MTimer::ThreadProc1()
{
	DWORD dwRet = 0;
	m_uTimerID = ::SetTimer(NULL, 0, m_uElapse, onTimerProc);
	if (0 == m_uTimerID)
	{
		dwRet = ::GetLastError();
		std::cout << "create timer failed, error code:" << dwRet << std::endl;
		return dwRet;
	}

	{
		std::lock_guard<std::recursive_mutex> lck(s_mtxMap);
		s_mapUser[m_uTimerID] = m_funTimerProc1;
	}

	m_bStart = true;
	MSG   msg;
	while (m_bStart && GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_TIMER)
		{
			DispatchMessage(&msg);
		}
	}

	return dwRet;
}

void CALLBACK MTimer::onTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	{
		std::lock_guard<std::recursive_mutex> lck(s_mtxMap);
		if (s_mapUser.find((UINT)idEvent) != s_mapUser.end())
		{
			s_mapUser[idEvent](idEvent);
		}
	}

}

void MTimer::stop_timer()
{
	if (m_hThread)
	{
		{
			std::lock_guard<std::recursive_mutex> lck(s_mtxMap);
			s_mapUser.erase(m_uTimerID);
		}
		::KillTimer(NULL, m_uTimerID);
		m_uTimerID = 0;

		m_bStart = false;
		if (WAIT_TIMEOUT == ::WaitForSingleObject(m_hThread, 1000))
		{
			::CloseHandle(m_hThread);
		}
		m_hThread = NULL;
	}
	else
		std::cout << "timer has stoped!" << std::endl;
}
