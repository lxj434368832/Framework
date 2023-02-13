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

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

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
	if ('/' == path[iCount - 1] || '\\' == path[iCount - 1])	//����β�˵�б�ܻ�б��
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
		path[index + 1] = 0;		//����β���ġ�/����\��
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
		printf("%s%d:����Ϊ�գ�\n",__FUNCTION__,__LINE__);
		return strTemp;
	}

	iCount = strlen(lpName);
	if (0 == iCount)
	{
		printf("%s%d:����Ϊ0 ��\n", __FUNCTION__, __LINE__);
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
		printf("������Դ�ļ���ʧ�ܣ�Դ�ļ���Ϊ:%s", strName);
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
		printf("%s%d:����Ϊ�գ�\n", __FUNCTION__, __LINE__);
		return strTemp;
	}

	iCount = strlen(lpName);
	if (0 == iCount)
	{
		printf("%s%d:����Ϊ0 ��\n", __FUNCTION__, __LINE__);
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
		printf("�����ļ�ʧ�ܣ�����ֵΪ:%d\n",iRet);
		return FALSE;
	}
	return TRUE;
}

bool FileHandle::CopyDirectory(LPCTSTR lpSource, LPCTSTR lpDest)
{
	/* ��Ҫ�õ�MFC
	CreateDirectory(lpDest, NULL); //����Ŀ���ļ���  
	//AfxMessageBox("�����ļ���"+target);  
	CFileFind finder;
	std::string path;
	path.append(lpSource).append(TEXT("/*.*"));
	//AfxMessageBox(path);  
	BOOL bWorking = finder.FindFile(path.c_str());
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		//AfxMessageBox(finder.GetFileName());  
		if (finder.IsDirectory() && !finder.IsDots())//���ļ��� ���� ���Ʋ��� . �� ..  
		{
			//�ݹ鴴���ļ���+"/"+finder.GetFileName()
			CopyDirectory(finder.GetFilePath(), std::string(lpDest).append("/").append(finder.GetFileName()).c_str());
		}
		else//���ļ� ��ֱ�Ӹ��� 
		{
			//AfxMessageBox("�����ļ�"+finder.GetFilePath());//+finder.GetFileName()  
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
		printf("�����ļ�ʧ�ܣ�����ֵΪ:%d\n", iRet);
		return FALSE;
	}
	return TRUE;
}

bool FileHandle::DeleteDirectory(LPCTSTR lpSource)
{
	/*	��Ҫ�õ�MFC��
	CFileFind finder;
	CString path;
	path.Format(_T("%s/*.*"), lpSource);
	BOOL bWorking = finder.FindFile(path);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots())//�����ļ���
		{
			DeleteDirectory(finder.GetFilePath()); //�ݹ�ɾ���ļ���
			::RemoveDirectory(finder.GetFilePath());
		}
		else//�����ļ�
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
		//�ж��Ƿ�����Ŀ¼  
		if (fileInfo.attrib & _A_SUBDIR)
		{
			//���������Ҫ  
			if ((strcmp(fileInfo.name, ".") != 0) && (strcmp(fileInfo.name, "..") != 0))
			{
				curr = std::string(lpDir) + "\\" + fileInfo.name;
				printf("folder:%s",curr.c_str());
				
				auto files = GetAllFile(curr.c_str(),filter);
				listFiles.insert(listFiles.end(), files.begin(), files.end());
			}
		}
		else
		{
			curr = std::string(lpDir).append("\\").append(fileInfo.name);
			listFiles.push_back(curr);
			printf("�ҵ��ļ�%s\n", curr.c_str());
		}
	} while (0 == _findnext(handle, &fileInfo));
	_findclose(handle);

	return listFiles;
}

void FileHandle::GetDirFileList(LPCTSTR lpRootDir, std::vector<tstring>& fileArray)
{
	if (nullptr == lpRootDir || 0 == _tcsclen(lpRootDir)) return;

	TCHAR lpTmpPath[MAX_PATH];
	TCHAR lpCrtDir[MAX_PATH];
	ZeroMemory(&lpTmpPath, sizeof(lpTmpPath));
	ZeroMemory(lpCrtDir, sizeof(lpCrtDir));

	BOOL bRet = TRUE;
	static int nFileSize = 0;
	_tcscpy_s(lpTmpPath, MAX_PATH, lpRootDir);
	::PathAddBackslash(lpTmpPath);
	_tcscpy_s(lpCrtDir, MAX_PATH, lpTmpPath);
	_tcscat_s(lpTmpPath, _T("*"));

	WIN32_FIND_DATA fd;
	ZeroMemory(&fd, sizeof(fd));
	HANDLE hFile = FindFirstFile(lpTmpPath, &fd);

	while (INVALID_HANDLE_VALUE != hFile && bRet)
	{
		//�ж��Ƿ�����Ŀ¼  
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//���������Ҫ  
			if (_tcscmp(fd.cFileName, _T(".")) && _tcscmp(fd.cFileName, _T("..")))
			{
				_tcscpy_s(lpTmpPath, MAX_PATH, lpCrtDir);
				_tcscat_s(lpTmpPath, fd.cFileName);
				GetDirFileList(lpTmpPath, fileArray);
			}
		}
		else
		{
			tstring strFile = tstring(lpCrtDir).append(fd.cFileName);
			fileArray.push_back(strFile);
		}
		bRet = FindNextFile(hFile, &fd);
	}
	FindClose(hFile);
}


/*
* DWORD EnumerateFileInDirectory(LPSTR szPath)
* ���ܣ�����Ŀ¼�µ��ļ�����Ŀ¼������ʾ�ļ����ļ������ء����ܵ�����
*
* ������LPSTR szPath��Ϊ�������·��
*
* ����ֵ��0����ִ����ɣ�1�����ʹ���
*/
DWORD EnumerateFileInDirectory(LPTSTR szPath)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile;
	TCHAR szFilePath[MAX_PATH];
	
	// ���������Ŀ¼���ļ���·�����ַ�����ʹ��ͨ���"*"
	_tcscpy_s(szFilePath, szPath);
	// ע�͵Ĵ���������ڲ��������ԡ�.txt����β���ļ�
	// lstrcat(szFilePath, "\\*.txt");
	_tcscat_s(szFilePath, _T("\\*"));

	// ���ҵ�һ���ļ�/Ŀ¼����ò��Ҿ��
	hListFile = FindFirstFile(szFilePath, &FindFileData);
	// �жϾ��
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		_tprintf_s(_T("����%d\n"), GetLastError());
		return 1;
	}
	else
	{
		do
		{
			/* ���������ʾ������Ŀ¼���ϼ�Ŀ¼�ġ�.���͡�..����
			����ʹ��ע�Ͳ��ֵĴ������
			if(lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 ||
			lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)
			{
				continue;
			}
			*/

			// ��ӡ�ļ�����Ŀ¼��
			_tprintf_s(_T("%Ts\t\t"), FindFileData.cFileName);
			// �ж��ļ����ԣ��Ƿ�Ϊ�����ļ������ļ���
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
			{
				_tprintf_s(_T("<����> "));
			}
			// �ж��ļ����ԣ��Ƿ�Ϊ�����ļ����ļ���
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			{
				_tprintf_s(_T("<����> "));
			}
			// �ж��ļ����ԣ��Ƿ�ΪĿ¼
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				_tprintf_s(_T("<DIR> "));
			}
			// ���߿ɸ����ļ����Ա��е�����������ӡ��ж��ļ�����
			_tprintf_s(_T("\n"));
		} while (FindNextFile(hListFile, &FindFileData));
	}
	FindClose(hListFile);
	return 0;
}
