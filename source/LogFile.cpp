#include "../include/LogFile.h"
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
	sprintf_s(m_szLineLog, "[%04d-%02d-%02d %02d:%02d:%02d %03d|",
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

LogFile* LogFile::s_instance = nullptr;

LogFile::LogFile()
{
	m_bStart = true;
	CheckFileName();
	m_pThread = new std::thread(&LogFile::WriteLogThread, this);
	//��ʱ10����
	m_unTimerId = ::timeSetEvent(600000, 60000, TimeCallback, (DWORD_PTR)this, TIME_PERIODIC);;
	if (0 == m_unTimerId)
	{
		std::cout << "�����ļ���鶨ʱ��ʧ��!" << std::endl;
	}

	if (nullptr != s_instance)
	{
		delete s_instance;
	}
	s_instance = this;

}

LogFile::~LogFile()
{
	if (false == m_bStart) return;
	s_instance = nullptr;
	m_bStart = false;
	::timeKillEvent(m_unTimerId);

	::OutputDebugString("׼���˳���־�̣߳�\n");
	m_cvConsumer.notify_all();
	if (m_pThread->joinable())
		m_pThread->join();
	::OutputDebugString("�ɹ��˳���־�̣߳�\n");
	delete m_pThread;
}

void LogFile::CheckFileName()
{
	tm sNow;
	time_t ulNow = ::time(nullptr);
	::localtime_s(&sNow, &ulNow);

	if (sNow.tm_yday != m_iDayOfYear)
	{
		m_iDayOfYear = sNow.tm_yday;
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
				std::cout << "������־Ŀ¼ʧ�ܣ�" << std::endl;
		}

		m_mutexLogList.lock();
		sprintf_s(m_szFileName, MAX_PATH, "%s%s%04d%02d%02d%02d%02d%02d.log",
			szFullPath, szFileName,
			sNow.tm_year + 1900, sNow.tm_mon + 1, sNow.tm_mday,
			sNow.tm_hour, sNow.tm_min, sNow.tm_sec);
		m_mutexLogList.unlock();
		std::cout << "��ǰ��־�ļ�Ϊ:" << m_szFileName << std::endl;
	}
}

void LogFile::AddLog(std::string &strLog)
{
	if (false == m_bStart) return;
	m_mutexLogList.lock();
	m_logList.push(strLog);
	m_cvConsumer.notify_one();
	m_mutexLogList.unlock();

#ifdef _DEBUG
	std::cout << strLog.c_str();
#endif // _DEBUG

#ifdef WIN32
	::OutputDebugString(strLog.c_str());
#endif
}

void LogFile::WriteLogThread()
{
	while (m_bStart ||  !m_logList.empty())
	{
		std::string strLog;
		{
			std::unique_lock<std::mutex> lck(m_mutexLogList);
			if (m_logList.empty())
			{
				m_cvConsumer.wait(lck);
			}
			if (m_logList.empty()) continue;
			strLog = m_logList.front();
			m_logList.pop();
		}
		WriteLog(strLog);
	}
}

void LogFile::WriteLog(std::string &strLog)
{
	FILE	*pFile;
	if (0 != fopen_s(&pFile, m_szFileName, "a"))
	{
		std::cout << "���ļ�:" << m_szFileName << "ʧ��!" << std::endl;
		return;
	}

	fprintf(pFile, strLog.c_str());
	fclose(pFile);
}

void CALLBACK LogFile::TimeCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	LogFile *pThis = (LogFile *)dwUser;
	if (uTimerID == pThis->m_unTimerId)
	{
		pThis->CheckFileName();
	}
}
