#include "../include/MTimer.h"
#include "../include/LogFile.h"

MTimer::MTimer()
{
}

MTimer::~MTimer()
{
	Stop();
}

bool MTimer::Start(unsigned uInterval, funTimerProc lpTimerFunc, bool bStartCall)
{
	if (m_pTimerThread)
	{
		Stop();
	}

	m_funTimerProc = lpTimerFunc;

	m_pTimerThread = new std::thread(&MTimer::TimerCallback, this, uInterval, bStartCall);
	return (nullptr != m_pTimerThread);
}

#define thread_id std::hash<std::thread::id>()(std::this_thread::get_id())
void MTimer::TimerCallback(unsigned uInterval, bool bStartCall)
{
	LOGM("定时器线程：%d启动。", thread_id);
	if (bStartCall && m_funTimerProc)
		m_funTimerProc();

	m_hTimerEvent = ::CreateEvent(NULL, false, false, NULL);
	if (INVALID_HANDLE_VALUE == m_hTimerEvent)
	{
		loge() << "创建定时器事件失败！";
		return;
	}

	while (true)
	{
		DWORD dwRet = ::WaitForSingleObject(m_hTimerEvent, uInterval);
		if (WAIT_OBJECT_0 == dwRet)
		{
			logm() << "正常退出定时器线程:"<< std::this_thread::get_id();
			break;
		}
		else if (WAIT_TIMEOUT == dwRet)
		{
			if (m_funTimerProc)
				m_funTimerProc();
		}
		else
		{
			LOGM("定时器线程:%d 发生错误：%d。", thread_id, ::GetLastError());
			break;
		}
	}
}

void MTimer::Stop()
{
	if (m_hTimerEvent)	
		::SetEvent(m_hTimerEvent);

	if (m_pTimerThread && m_pTimerThread->joinable())
		m_pTimerThread->join();

	if (m_hTimerEvent)
	{
		::CloseHandle(m_hTimerEvent);
		m_hTimerEvent = INVALID_HANDLE_VALUE;
	}

	if (m_pTimerThread)
	{
		delete m_pTimerThread;
		m_pTimerThread = nullptr;
	}
}

