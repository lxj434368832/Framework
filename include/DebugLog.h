#pragma once
#pragma once
/*******************************************************************
* @版权信息：
* @文件名称：Log.h
* @摘    要：日志操作定义头文件
* @作    者：明巧文
* @当前版本：1.0.0
* @日    期：2021年6月10日
* @备    注：线程安全日志类,日志的最大大小为8M
*******************************************************************/

#include <stdio.h>
#include <string>
#include <wtypes.h>

//写日志函数
//void LOG(const char* format, ...);
void LOG(LPCSTR lpFun, int nLine, LPCSTR lpLevel, LPCSTR format, ...);

void LOG(LPCWSTR lpFun, int nLine, LPCWSTR lpLevel, LPCWSTR format, ...);

void DeleteLibLog();

#define LOGM(format, ...)  LOG(__FUNCTION__, __LINE__, "Info", format, ##__VA_ARGS__)

#define WLOGM(format, ...)  LOG(_T(__FUNCTION__), __LINE__, _T("Info"), format, ##__VA_ARGS__)

/*
#define LOGM(format, ...)  do{  \
	char szFunInfo[MAX_PATH];	\
	sprintf_s(szFunInfo, MAX_PATH, "[%s:%d]<Info> %s\n",__FUNCTION__,__LINE__, format);  \
	LOG(szFunInfo, ##__VA_ARGS__); }while(0)

#define LOGD(format, ...)  do{  \
	char szFunInfo[MAX_PATH];	\
	sprintf_s(szFunInfo, MAX_PATH, "[%s:%d]<Debug> %s\n",__FUNCTION__,__LINE__, format);  \
	LOG(szFunInfo, ##__VA_ARGS__); }while(0)

#define LOGW(format, ...)  do{  \
	char szFunInfo[MAX_PATH];	\
	sprintf_s(szFunInfo, MAX_PATH, "[%s:%d]<Warn> %s\n",__FUNCTION__,__LINE__, format);  \
	LOG(szFunInfo, ##__VA_ARGS__); }while(0)

#define LOGE(format, ...)  do{  \
	char szFunInfo[MAX_PATH];	\
	sprintf_s(szFunInfo, MAX_PATH, "[%s:%d]<Error> %s\n",__FUNCTION__,__LINE__, format);  \
	LOG(szFunInfo, ##__VA_ARGS__); }while(0)


#define WLOGM(format, ...)  do{  \
	wchar szFunInfo[MAX_PATH];	\
	sprintf_s(szFunInfo, MAX_PATH, L"[%s:%d]<Info> %s\n",__FUNCTION__,__LINE__, format);  \
	LOG(szFunInfo, ##__VA_ARGS__); }while(0)

#define WLOGD(format, ...)  do{  \
	wchar szFunInfo[MAX_PATH];	\
	sprintf_s(szFunInfo, MAX_PATH, L"[%s:%d]<Debug> %s\n",__FUNCTION__,__LINE__, format);  \
	LOG(szFunInfo, ##__VA_ARGS__); }while(0)

#define WLOGW(format, ...)  do{  \
	wchar szFunInfo[MAX_PATH];	\
	sprintf_s(szFunInfo, MAX_PATH, L"[%s:%d]<Warn> %s\n",__FUNCTION__,__LINE__, format);  \
	LOG(szFunInfo, ##__VA_ARGS__); }while(0)

#define WLOGE(format, ...)  do{  \
	wchar szFunInfo[MAX_PATH];	\
	sprintf_s(szFunInfo, MAX_PATH, L"[%s:%d]<Error> %s\n",__FUNCTION__,__LINE__, format);  \
	LOG(szFunInfo, ##__VA_ARGS__); }while(0)
*/