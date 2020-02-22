#include "../include/LibLog.h"
#include <thread>
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <io.h>
#include <iostream>
#include <memory>
#include <windows.h>


struct SLogInfo
{
	std::string strLibName;
	std::string strLog;
};

struct SLogPrivate
{
	bool					bStart;
	std::mutex				mutexBuffer;
	char					szBuffer[LOG_BUFFER_SIZE]; 
	std::string				strAppDir;
	DWORD					dwTickCount;
	std::thread				*pThread;
	std::map<std::string, std::string> mapFilePath;
	std::queue<SLogInfo>	queueLogList;
	std::mutex				mutexLogList;
	std::condition_variable	cvConsumer;

	int						m_iDayOfYear;

	SLogPrivate();
	~SLogPrivate();
};


static std::unique_ptr<SLogPrivate>  s_d(new SLogPrivate);

void AddLog(SLogInfo& info);
void WriteLogThread(SLogPrivate*);
void WriteLog(std::string &strLibName, std::string& strLog);
void CheckFilePath(std::string strLibName);

SLogPrivate::SLogPrivate()
{
	bStart = true;
	strAppDir = GetAppDir();
	dwTickCount = ::GetTickCount();
	pThread = new std::thread(&WriteLogThread, this);

	m_iDayOfYear = 0;
}

SLogPrivate::~SLogPrivate()
{
	bStart = false;
	::OutputDebugString("准备退出日志线程！\n");
	cvConsumer.notify_all();
	if (pThread->joinable())
		pThread->detach();
	::OutputDebugString("成功退出日志线程！\n");
	delete pThread;
}

std::string GetAppDir()
{
	char szFullPath[MAX_PATH];
	char szDrive[32];
	char szDir[MAX_PATH];

	::GetModuleFileName(nullptr, szFullPath, MAX_PATH);
	::_splitpath_s(szFullPath, szDrive, 32, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
	return std::string(szDrive).append(szDir);
}

void LOG(const char * szLibName, const char* format, ...)
{
	if (nullptr == s_d) return;

	//2、判断lib目录是否存在
	std::string strLogDir(s_d->strAppDir + szLibName);
	if (-1 == _access(strLogDir.data(), 0)) return;

	SLogInfo info;
	info.strLibName = szLibName;

	//3、写入时间信息
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	char szTime[MAX_PATH];
    sprintf_s(szTime, "[%04d-%02d-%02d %02d:%02d:%02d %03d]",
		tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
	info.strLog.append(szTime);

	//4、写入日志内容
	va_list argList;
    va_start(argList, format);

	s_d->mutexBuffer.lock();
	vsprintf_s(s_d->szBuffer, format, argList);
	info.strLog.append(s_d->szBuffer);
	s_d->mutexBuffer.unlock();

	va_end(argList);
	
	//5、将日志信息添加到队列中
	AddLog(info);
}

void AddLog(SLogInfo& info)
{
	s_d->mutexLogList.lock();
	s_d->queueLogList.push(info);
	s_d->cvConsumer.notify_one();
	s_d->mutexLogList.unlock();
}

void WriteLogThread(SLogPrivate* d)
{
	while (d->bStart)
	{
		SLogInfo logInfo;
		{
			std::unique_lock<std::mutex> lck(d->mutexLogList);
			if (d->queueLogList.empty())
			{
				d->cvConsumer.wait(lck);
			}
			if (d->queueLogList.empty()) continue;
			logInfo = d->queueLogList.front();
			d->queueLogList.pop();
		}
		WriteLog(logInfo.strLibName, logInfo.strLog);
	}

	::OutputDebugString("日志线程已经退出！\n");
}

void WriteLog(std::string &strLibName, std::string& strLog)
{
	std::string strLogPath = s_d->mapFilePath[strLibName];
	if (strLogPath.empty())
	{
		CheckFilePath(strLibName);
		strLogPath = s_d->mapFilePath[strLibName];
	}

	//每隔一个小时检测下文件名
	if (3600000 < ::GetTickCount() - s_d->dwTickCount)
	{
		CheckFilePath("");
	}

	FILE	*pFile;
	errno_t err = fopen_s(&pFile, strLogPath.data(), "a");
	if (0 != err)
	{
		std::cout << "打开文件:" << strLogPath << "失败,error:" << err << std::endl;
		return;
	}

	fprintf(pFile, strLog.c_str());
	fclose(pFile);
}

void CheckFilePath(std::string strLibName)
{
	char szLogPath[MAX_PATH];
	//获取当前时间
	tm sNow;
	time_t ulNow = ::time(nullptr);
	::localtime_s(&sNow, &ulNow);

	if (strLibName.empty())
	{
		s_d->dwTickCount = ::GetTickCount();

		if (0 == s_d->m_iDayOfYear)
		{
			s_d->m_iDayOfYear = sNow.tm_yday;
		}
		else if (sNow.tm_yday != s_d->m_iDayOfYear)
		{
			s_d->m_iDayOfYear = sNow.tm_yday;

			for (auto itValue : s_d->mapFilePath)
			{
				strLibName = itValue.first;
				sprintf_s(szLogPath, MAX_PATH, "%s%s\\%s%04d%02d%02d%2d%2d%2d.log",
					s_d->strAppDir.data(), strLibName.data(), strLibName.data(),
					sNow.tm_year + 1900, sNow.tm_mon + 1, sNow.tm_mday,
					sNow.tm_hour, sNow.tm_min, sNow.tm_sec);
				s_d->mapFilePath[strLibName] = szLogPath;
			}
		}
	}
	else
	{
		sprintf_s(szLogPath, MAX_PATH, "%s%s\\%s%04d%02d%02d%2d%2d%2d.log",
			s_d->strAppDir.data(), strLibName.data(), strLibName.data(),
			sNow.tm_year + 1900, sNow.tm_mon + 1, sNow.tm_mday,
			sNow.tm_hour, sNow.tm_min, sNow.tm_sec);

		s_d->mapFilePath[strLibName] = szLogPath;
	}
}
