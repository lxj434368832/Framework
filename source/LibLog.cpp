#include "../include/LibLog.h"
#include <mutex>
#include <io.h>
#include <windows.h>


std::mutex				g_mutex;

std::string GetAppDir()
{
	char szFullPath[MAX_PATH];
	char szDrive[32];
	char szDir[MAX_PATH];

	::GetModuleFileName(nullptr, szFullPath, MAX_PATH);
	::_splitpath_s(szFullPath, szDrive, 32, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
	return std::string(szDrive).append(szDir);
}

void LOG(const char * szLibName, char* format, ...)
{
	static std::string strAppDir;

	if (strAppDir.empty())	strAppDir = GetAppDir();

	std::string strLogDir(strAppDir + szLibName);
	if (-1 == _access(strLogDir.data(), 0)) return;

	//获取当前时间
	SYSTEMTIME tm;
	GetLocalTime(&tm);

	char szLogPath[MAX_PATH];
	sprintf_s(szLogPath, MAX_PATH, "%s\\%s%04d%02d%02d.log", strLogDir.data(),
		szLibName, tm.wYear, tm.wMonth, tm.wDay);

	/* 文件处理 */
	g_mutex.lock();
	FILE *pFile = NULL;
	if (0 != fopen_s(&pFile, szLogPath, "a"))
	{
		g_mutex.unlock();
		return;
	}

    fprintf(pFile, "[%04d-%02d-%02d %02d:%02d:%02d %03d]", 
		tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);

	//处理参数
	va_list argList;
    va_start(argList, format);
    vfprintf(pFile, format, argList);
    fclose(pFile);

	g_mutex.unlock();
}
