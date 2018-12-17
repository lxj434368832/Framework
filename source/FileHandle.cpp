#include "../include/FileHandle.h"
#include <string>
#include <time.h>
#include <iostream>
#include <fstream>  
#include <iosfwd>
#include <direct.h>  
#include <io.h>  
#include <shlobj.h>  
#include <sys/stat.h>  
#include <sys/types.h>  

#pragma comment (lib,"Version.lib")
#pragma comment (lib,"shell32.lib")


FileHandle::FileHandle()
{
}

FileHandle::~FileHandle()
{
}

void FileHandle::GetFileVersion(LPCSTR FilePath, LPSTR buffer)
{
	std::string asVer = "";
	VS_FIXEDFILEINFO *pVsInfo;
	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);

	buffer[0] = 0;
	int iVerInfoSize = GetFileVersionInfoSize(FilePath, NULL);
	if (iVerInfoSize != 0)
	{
		char *pBuf = NULL;
		while (!pBuf)
		{
			pBuf = new char[iVerInfoSize];
		}
		if (GetFileVersionInfo(FilePath, 0, iVerInfoSize, pBuf))
		{
			if (VerQueryValue(pBuf, "\\", (void **)&pVsInfo, &iFileInfoSize))
			{
				sprintf_s(buffer, MAX_PATH, "%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS),
					LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
			}
		}
		delete pBuf;
	}
}

std::string FileHandle::GetParentDir(LPCSTR strPath)
{
	std::string strDir;
	char path[MAX_PATH];

	if (strPath == NULL)
		return strDir;

	int index = 0, iCount = strlen(strPath);
	if (iCount > MAX_PATH)
	{
		::printf("error path:%s", strPath);
		return strDir;
	}

	memcpy(path, strPath, iCount);
	if ('/' == path[iCount - 1] || '\\' == path[iCount - 1])	//处理尾端的斜杠或反斜杠
	{
		path[iCount - 1] = 0;
	}

	for (int i = 0; i < iCount; i++)
	{
		if ('/' == path[i] || '\\' == path[i])
		{
			index = i;
		}
	}
	if (0 != index)
	{
		path[index + 1] = 0;		//保留尾部的‘/’或‘\’
		strDir.append(path);
	}
	return strDir;

}

#ifdef WIN32
#define ACCESS(fileName,accessMode) _access(fileName,accessMode)
#define MKDIR(path) _mkdir(path)
#else
#define ACCESS(fileName,accessMode) access(fileName,accessMode)
#define MKDIR(path) mkdir(path,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#endif

int FileHandle::CreateDir(LPCSTR strDir)
{
	int iRet = 0;
	std::string strPath = strDir;
	int iPathLen = strPath.length();
	if (iPathLen > MAX_PATH)
		return -1;

	if (strPath[iPathLen - 1] != '\\' &&  strPath[iPathLen - 1] != '/')
		strPath.append(1, '\\');

	std::string tmpDirPath;
	for (int i = 0; i < iPathLen; i++)
	{
		tmpDirPath.append(1, strPath[i]);
		if (('\\' == tmpDirPath[i] || '/' == tmpDirPath[i]) &&
			0 != ACCESS(tmpDirPath.c_str(), 0))
		{
			iRet = MKDIR(tmpDirPath.c_str());
			if (iRet != 0)
			{
				return iRet;
			}
		}
	}

	return iRet;
}

std::string FileHandle::RenameWithTimeStamp(LPCSTR lpName)
{
	time_t _tm;
	tm stm;
	char strNewName[MAX_PATH];
	int iCount = 0;
	char  strName[MAX_PATH];
	std::string strTemp;

	ZeroMemory(strName, MAX_PATH);

	if (NULL == lpName)
	{
		printf("%s%d:名字为空！\n",__FUNCTION__,__LINE__);
		return strTemp;
	}

	iCount = strlen(lpName);
	if (0 == iCount)
	{
		printf("%s%d:长度为0 ！\n", __FUNCTION__, __LINE__);
		return strTemp;
	}

	strcpy_s(strName, lpName);
	if ('/' == strName[iCount - 1] || '\\' == strName[iCount - 1])
	{
		strName[iCount - 1] = 0;
	}

	::time(&_tm);
	localtime_s(&stm, &_tm);
	sprintf_s(strNewName, "%s_%02d%02d%02d%02d%02d%02d\\", strName, stm.tm_year,
		stm.tm_mon, stm.tm_mday, stm.tm_hour, stm.tm_min, stm.tm_sec);

	if (-1 == rename(strName, strNewName))
	{
		printf("重命名源文件夹失败！源文件夹为:%s", strName);
		return strTemp;
	}

	strTemp.append(strNewName);
	return strTemp;

}

std::string FileHandle::AddTimeStapToName(LPCSTR lpName)
{
	time_t _tm;
	tm stm;
	char strNewName[MAX_PATH];
	int iCount = 0;
	char  strName[MAX_PATH];
	std::string strTemp;

	ZeroMemory(strName, MAX_PATH);

	if (NULL == lpName)
	{
		printf("%s%d:名字为空！\n", __FUNCTION__, __LINE__);
		return strTemp;
	}

	iCount = strlen(lpName);
	if (0 == iCount)
	{
		printf("%s%d:长度为0 ！\n", __FUNCTION__, __LINE__);
		return strTemp;
	}

	strcpy_s(strName, lpName);
	if ('/' == strName[iCount - 1] || '\\' == strName[iCount - 1])
	{
		strName[iCount - 1] = 0;
	}

	::time(&_tm);
	localtime_s(&stm, &_tm);
	sprintf_s(strNewName, "%s_%02d%02d%02d%02d%02d%02d\\", strName, stm.tm_year,
		stm.tm_mon, stm.tm_mday, stm.tm_hour, stm.tm_min, stm.tm_sec);

	strTemp.append(strNewName);
	return strTemp;
}

BOOL FileHandle::CopyFile(LPCTSTR lpSource, LPCTSTR lpDest)
{
	char lpCmd[512];
	int iRet = 0;
	sprintf_s(lpCmd, "cmd /c echo D | xcopy \"%s\" \"%s\" /a /I /Y /C", lpSource, lpDest);
	iRet = WinExec(lpCmd, SW_HIDE);
	if (iRet < 31)
	{
		printf("拷贝文件失败！返回值为:%d\n",iRet);
		return FALSE;
	}
	return TRUE;
}

bool FileHandle::CopyDirectory(LPCTSTR lpSource, LPCTSTR lpDest)
{
	/* 需要用到MFC
	CreateDirectory(lpDest, NULL); //创建目标文件夹  
	//AfxMessageBox("创建文件夹"+target);  
	CFileFind finder;
	std::string path;
	path.append(lpSource).append(TEXT("/*.*"));
	//AfxMessageBox(path);  
	BOOL bWorking = finder.FindFile(path.c_str());
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		//AfxMessageBox(finder.GetFileName());  
		if (finder.IsDirectory() && !finder.IsDots())//是文件夹 而且 名称不含 . 或 ..  
		{
			//递归创建文件夹+"/"+finder.GetFileName()
			CopyDirectory(finder.GetFilePath(), std::string(lpDest).append("/").append(finder.GetFileName()).c_str());
		}
		else//是文件 则直接复制 
		{
			//AfxMessageBox("复制文件"+finder.GetFilePath());//+finder.GetFileName()  
			::CopyFile(finder.GetFilePath(), std::string(lpDest).append("/").append(finder.GetFileName()).c_str(), FALSE);
		}
	}
	*/
	return true;
}

BOOL FileHandle::DeleteFile(LPCTSTR lpPath)
{
	char lpCmd[512];
	int iRet = 0;
	sprintf_s(lpCmd, "cmd /c echo D | rd /S /Q \"%s\"", lpPath);
	iRet = WinExec(lpCmd, SW_HIDE);
	if (iRet < 31)
	{
		printf("拷贝文件失败！返回值为:%d\n", iRet);
		return FALSE;
	}
	return TRUE;
}

bool FileHandle::DeleteDirectory(LPCTSTR lpSource)
{
	/*	需要用到MFC库
	CFileFind finder;
	CString path;
	path.Format(_T("%s/*.*"), lpSource);
	BOOL bWorking = finder.FindFile(path);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots())//处理文件夹
		{
			DeleteDirectory(finder.GetFilePath()); //递归删除文件夹
			::RemoveDirectory(finder.GetFilePath());
		}
		else//处理文件
		{
			::DeleteFile(finder.GetFilePath());
		}
	}
	::RemoveDirectory(lpSource);
*/
	return true;
}

std::list<std::string> FileHandle::GetAllFile(LPCTSTR lpDir, LPCTSTR filter)
{
	std::list<std::string> listFiles;
	_finddata_t fileInfo;
	std::string  curr = std::string(lpDir).append("\\").append(filter);

	int handle = _findfirst(curr.c_str(), &fileInfo);

	if (-1 == handle)
	{
		printf("can not match the folder path! \n");
		return listFiles;
	}

	do 
	{
		//判断是否有子目录  
		if (fileInfo.attrib & _A_SUBDIR)
		{
			//这个语句很重要  
			if ((strcmp(fileInfo.name, ".") != 0) && (strcmp(fileInfo.name, "..") != 0))
			{
				curr = std::string(lpDir) + "\\" + fileInfo.name;
				printf("folder:%s",curr.c_str());
				GetAllFile(curr.c_str(),filter);
			}
		}
		else
		{
			curr = std::string(lpDir).append("\\").append(fileInfo.name);
			listFiles.push_back(curr);
			printf("找到文件%s\n", curr.c_str());
		}
	} while (0 == _findnext(handle, &fileInfo));
	_findclose(handle);

	return listFiles;
}
