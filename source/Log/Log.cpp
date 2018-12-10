#include "../../include/Log/Log.h"
#include<mutex>
#include <windows.h>
/*
WriteLog: 写日志函数---如果日志文件超过指定大小 则从0开始
*/



int g_nLogLevel = LOG_DEBUG;
int g_nLogFileSize = MAX_LOG_FILE_SIZE;

char g_szFileName[MAX_PATH] = { 0 };
std::recursive_mutex g_cs;
std::recursive_mutex g_avcs;
bool g_bInit=false;

void InitLog()
{
	char path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);
	sprintf_s(g_szFileName, "%s//DATAEXCHANGE.log", path);

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
    g_cs.lock();

    FILE *pFile = NULL;
    int dwFileSize = 0;
    va_list arg;

    SYSTEMTIME tm;
	char strTime[MAX_LOG_ROW_SIZE] = { 0 };
	//获取当前时间
	GetLocalTime(&tm);
	sprintf_s(strTime, MAX_LOG_ROW_SIZE, "%04d-%02d-%02d %02d:%02d:%02d %03d",
		tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);

    if (t > g_nLogLevel) goto END_LOG;

    /* 文件处理 */

    fopen_s(&pFile, g_szFileName, "a+");
    if (pFile == NULL)
    {
        goto END_LOG;
    }

    //限制大小
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
    va_start(arg, format);
    vfprintf(pFile, format, arg);
    fprintf(pFile, "\n");
    fflush(pFile);
    va_end(arg);
    fclose(pFile);

END_LOG:
    g_cs.unlock();
}
