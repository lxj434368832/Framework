#include "../include/LogFile.h"
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <wtypes.h>
#include <iostream>
#include <io.h>
#include <direct.h>
#ifdef WIN32
#include <Windows.h>
#endif

#pragma comment(lib,"Winmm.lib")

SingleLog::SingleLog()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	sprintf_s(m_szLineLog, "[%04d-%02d-%02d %02d:%02d:%02d %03d]",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
	m_strStream << m_szLineLog;
}

SingleLog::~SingleLog()
{
	m_strStream << "\n";
	LogFile *pLog = LogFile::GetInstance();
	if (pLog)
		pLog->AddLog(m_strStream.str());
}

void SingleLog::AddLog(char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	vsprintf_s(m_szLineLog, format, argList);
	va_end(argList);

	m_strStream << m_szLineLog;
}

class LogFileData
{
	friend class LogFile;

	bool			m_bStart;			//是否开始的标识
	std::thread		*m_pThread;
	std::queue<std::string>	m_logList;
	std::mutex				m_mutexLogList;
	std::condition_variable	m_cvConsumer;
	char					m_szFileName[MAX_PATH];
	int						m_iDayOfYear;
	UINT					m_unTimerId;
};

LogFile* LogFile::s_instance = nullptr;

void CALLBACK TimeCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	((LogFile *)dwUser)->Timeout(uTimerID);
}

LogFile::LogFile()
{
	d = new LogFileData;
	d->m_bStart = true;
	CheckFileName();
	d->m_pThread = new std::thread(&LogFile::WriteLogThread, this);
	//定时10分钟
	d->m_unTimerId = ::timeSetEvent(600000, 60000, TimeCallback, (DWORD_PTR)this, TIME_PERIODIC);;
	if (0 == d->m_unTimerId)
	{
		std::cout << "创建文件检查定时器失败!" << std::endl;
	}

	if (nullptr != s_instance)
	{
		delete s_instance;
	}
	s_instance = this;

}

LogFile::~LogFile()
{
	if (false == d->m_bStart) return;
	s_instance = nullptr;
	d->m_bStart = false;
	::timeKillEvent(d->m_unTimerId);
	
	char strMsg[128];
	sprintf_s(strMsg, "还有%d条日志没有写入文件，请等待...\n", m_logList.size());
	std::cout << strMsg;
	::OutputDebugString(strMsg);
	
	d->m_cvConsumer.notify_all();
	if (d->m_pThread->joinable())
		d->m_pThread->join();
	::OutputDebugString("成功退出日志线程！\n");
	std::cout << "成功退出日志线程！\n";
	delete d->m_pThread;

	delete d;
	d = nullptr;
}

void LogFile::Timeout(unsigned uTimerID)
{
	if (uTimerID == d->m_unTimerId)
	{
		CheckFileName();
	}
}

void LogFile::CheckFileName()
{
	tm sNow;
	time_t ulNow = ::time(nullptr);
	::localtime_s(&sNow, &ulNow);

	if (sNow.tm_yday != d->m_iDayOfYear)
	{
		d->m_iDayOfYear = sNow.tm_yday;
		char szFullPath[MAX_PATH];
		char szDrive[32];
		char szDir[MAX_PATH];
		char szFileName[64];

		::GetModuleFileName(nullptr, szFullPath, MAX_PATH);
		::_splitpath_s(szFullPath, szDrive, 32, szDir, MAX_PATH, szFileName, 64, nullptr, 0);
		sprintf_s(szFullPath, MAX_PATH, "%s%slog\\", szDrive, szDir);

		if (-1 == _access(szFullPath, 0))
		{
			if (0 != ::_mkdir(szFullPath))
				std::cout << "创建日志目录失败！" << std::endl;
		}

		d->m_mutexLogList.lock();
		sprintf_s(d->m_szFileName, MAX_PATH, "%s%s%04d%02d%02d%02d%02d%02d.log",
			szFullPath, szFileName,
			sNow.tm_year + 1900, sNow.tm_mon + 1, sNow.tm_mday,
			sNow.tm_hour, sNow.tm_min, sNow.tm_sec);
		d->m_mutexLogList.unlock();
		std::cout << "当前日志文件为:" << d->m_szFileName << std::endl;
	}
}

void LogFile::AddLog(std::string &strLog)
{
	if (false == d->m_bStart) return;
	d->m_mutexLogList.lock();
	d->m_logList.push(strLog);
	d->m_cvConsumer.notify_one();
	d->m_mutexLogList.unlock();

#ifdef _DEBUG
	std::cout << strLog.c_str();
#endif // _DEBUG

#ifdef WIN32
	::OutputDebugString(strLog.c_str());
#endif
}

void LogFile::WriteLogThread()
{
	while (d->m_bStart ||  !d->m_logList.empty())
	{
		std::string strLog;
		{
			std::unique_lock<std::mutex> lck(d->m_mutexLogList);
			if (d->m_logList.empty())
			{
				d->m_cvConsumer.wait(lck);
			}
			if (d->m_logList.empty()) continue;
			strLog = d->m_logList.front();
			d->m_logList.pop();
		}
		WriteLog(strLog);
	}
}

void LogFile::WriteLog(std::string &strLog)
{
	FILE	*pFile;
	errno_t err = fopen_s(&pFile, d->m_szFileName, "a");
	if (0 != err)
	{
		std::cout << "打开文件:" << d->m_szFileName << "失败,error:" << err << std::endl;
		return;
	}

	fprintf(pFile, strLog.c_str());
	fclose(pFile);
}

