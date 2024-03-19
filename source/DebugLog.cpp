#include "../include/DebugLog.h"
#include <thread>
#include <map>
#include <list>
#include <mutex>
#include <condition_variable>
#include <io.h>
#include <iostream>
#include <memory>
#include <windows.h>


#define LOG_BUFFER_SIZE 8 * 1024 * 1024	//8M


struct SLogPrivate
{
	bool					bStart;
	std::mutex				mutexBuffer;
	byte					btBuffer[LOG_BUFFER_SIZE];
	std::thread				* pThread;
	std::list<std::string>	logList;
	std::list<std::wstring>	wlogList;
	std::mutex				mutexLogList;
	std::condition_variable	cvConsumer;

	SLogPrivate();
	~SLogPrivate();
};

static std::unique_ptr<SLogPrivate>  s_d(new SLogPrivate);

void AddLog(const std::string& strLog);
void WriteLogThread(SLogPrivate*);
void WriteLog(std::list<std::string>& list);

SLogPrivate::SLogPrivate()
	: bStart(true)
{
	pThread = new std::thread(&WriteLogThread, this);
}

SLogPrivate::~SLogPrivate()
{
	bStart = false;
	::OutputDebugString("准备退出日志线程！\n");
	cvConsumer.notify_all();
	if (pThread->joinable())
	{
		pThread->join();
	}
	//pThread->detach();
	delete pThread;
	pThread = nullptr;
	::OutputDebugString("成功退出日志线程！\n");
}

/*
void LOG(const char* format, ...)
{
	if (nullptr == s_d) return;

	std::string strLog;

	//3、写入时间信息
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	char szTime[MAX_PATH];
	sprintf_s(szTime, "[%04d-%02d-%02d %02d:%02d:%02d %03d]",
		tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
	strLog.append(szTime);

	//4、写入日志内容
	va_list argList;
	va_start(argList, format);

	s_d->mutexBuffer.lock();
	vsprintf_s(s_d->btBuffer, format, argList);
	strLog.append(s_d->btBuffer);
	s_d->mutexBuffer.unlock();

	va_end(argList);

	//5、将日志信息添加到队列中
	AddLog(strLog);
}*/

void LOG(LPCSTR lpFun, int nLine, LPCSTR lpLevel, LPCSTR format, ...)
{
	if (nullptr == s_d) return;

	std::string strLog;

	//3、写入时间信息
	char szPrefixInfo[MAX_PATH];
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	sprintf_s(szPrefixInfo, MAX_PATH, "[%04d-%02d-%02d %02d:%02d:%02d %03d][%s:%d]<%s> ",
		tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds,
		lpFun, nLine, lpLevel);
	strLog.append(szPrefixInfo);

	//4、写入日志内容
	va_list argList;
	va_start(argList, format);

	s_d->mutexBuffer.lock();
	char* szBuf = reinterpret_cast<char*>(s_d->btBuffer);
	vsprintf_s(szBuf, LOG_BUFFER_SIZE, format, argList);
	strLog.append(szBuf);
	s_d->mutexBuffer.unlock();

	va_end(argList);

	strLog.append("\n");
	//5、将日志信息添加到队列中
	AddLog(strLog);
}

void LOG(LPCWSTR lpFun, int nLine, LPCWSTR lpLevel, LPCWSTR format, ...)
{
	if (nullptr == s_d) return;

	std::wstring strLog;

	//3、写入时间信息
	strLog.reserve(MAX_PATH);
	strLog.begin();
	wchar_t szPrefixInfo[MAX_PATH];
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	swprintf_s(szPrefixInfo, MAX_PATH, L"[%04d-%02d-%02d %02d:%02d:%02d %03d][%s:%d]<%s> ",
		tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds,
		lpFun, nLine, lpLevel);
	strLog.append(szPrefixInfo);

	//4、写入日志内容
	va_list argList;
	va_start(argList, format);

	s_d->mutexBuffer.lock();
	wchar_t* wszBuf = reinterpret_cast<wchar_t*>(s_d->btBuffer);
	vswprintf_s(wszBuf, LOG_BUFFER_SIZE / 2, format, argList);
	strLog.append(wszBuf);
	s_d->mutexBuffer.unlock();

	va_end(argList);

	strLog.append(L"\n");
	//5、将日志信息添加到队列中
	AddLog(strLog);
}

void DeleteLibLog()
{
	s_d.reset();
}

void AddLog(const std::string & strLog)
{
	s_d->mutexLogList.lock();
	s_d->logList.push_back(strLog);
	s_d->cvConsumer.notify_one();
	s_d->mutexLogList.unlock();
}

void WriteLogThread(SLogPrivate * d)
{
	while (d->bStart || false == d->logList.empty() || false == d->wlogList.empty())
	{
		std::list<std::string> logList;
		{
			std::unique_lock<std::mutex> lck(d->mutexLogList);

			if (d->logList.empty() && d->wlogList.empty())
			{
				d->cvConsumer.wait(lck);
			}
			if (d->logList.empty() && d->wlogList.empty()) continue;
			if(false == logList.empty()) logList.swap(d->logList);
		}

		WriteLog(logList);
	}

	//::OutputDebugString("日志线程已经退出！\n");
}

void WriteLog(std::list<std::string>& list)
{
	for (std::string& strLog : list)
	{
		::OutputDebugString(strLog.c_str());
	}
}
