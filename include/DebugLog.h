#pragma once
#pragma once
/*******************************************************************
* @��Ȩ��Ϣ��
* @�ļ����ƣ�Log.h
* @ժ    Ҫ����־��������ͷ�ļ�
* @��    �ߣ�������
* @��ǰ�汾��1.0.0
* @��    �ڣ�2021��6��10��
* @��    ע���̰߳�ȫ��־��,��־������СΪ8M
*******************************************************************/

#include <stdio.h>
#include <string>
#include <wtypes.h>

//д��־����
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