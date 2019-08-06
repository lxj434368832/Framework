#include "../../include/Log/Log.h"
#include<mutex>
#include <windows.h>


/*
WriteLog: 写日志函数---如果日志文件超过指定大小 则从0开始
*/
#define MLOG(tp, format, ...)  do{  \
    printf("[%s:%d]",__FUNCTION__,__LINE__);  \
    printf(format, ##__VA_ARGS__); printf("\n"); }while(0)



int g_nLogLevel = LOG_DEBUG;
int g_nLogFileSize = MAX_LOG_FILE_SIZE;

char g_szFileName[MAX_PATH] = { 0 };
std::recursive_mutex g_cs;
std::recursive_mutex g_avcs;
bool g_bInit=false;

void InitLog(char *pFileName)
{
	char path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);
	sprintf_s(g_szFileName, "%s/%s.log", path, pFileName);

	g_nLogLevel = LOG_NUM;
    g_nLogFileSize = MAX_LOG_FILE_SIZE;
    g_bInit = true;
}

void SetLogParam(int nLogLevel, int nLogFileSize)
{
    g_nLogLevel = nLogLevel;
    g_nLogFileSize = nLogFileSize;
}

void LOG(LOG_TYPE t, char* format, ...)
{
    if( false == g_bInit ) InitLog();
	if (t > g_nLogLevel) return;

    g_cs.lock();


	//获取当前时间
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	char strTime[MAX_LOG_ROW_SIZE] = { 0 };
	sprintf_s(strTime, MAX_LOG_ROW_SIZE, "%04d-%02d-%02d %02d:%02d:%02d %03d",
		tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);


	/* 文件处理 */
	FILE *pFile = NULL;
    fopen_s(&pFile, g_szFileName, "a+");
    if (pFile == NULL)
    {
        goto END_LOG;
    }

	//限制大小
	int dwFileSize = 0;
    fseek(pFile, 0, SEEK_END);
    dwFileSize = ftell(pFile);
    if (dwFileSize > g_nLogFileSize)
    {
        fclose(pFile);
        pFile = NULL;
        fopen_s(&pFile, g_szFileName, "w");
        dwFileSize = 0;
        if (pFile == NULL)
        {
            goto END_LOG;
        }
    }

    fprintf(pFile, "%s\t", strTime);
    switch (t)
    {
        case LOG_NONE:
            break;
        case LOG_ERROR:
            fprintf(pFile, "[ERR]");
            break;
        case LOG_RTM:
            fprintf(pFile, "[RTM]");
            break;
        case LOG_DEBUG:
            fprintf(pFile, "[DBG]");
            break;
    }

	//处理参数
	va_list argList;
    va_start(argList, format);
    vfprintf(pFile, format, argList);
    fprintf(pFile, "\n");
    fflush(pFile);
    fclose(pFile);

END_LOG:
    g_cs.unlock();
}
