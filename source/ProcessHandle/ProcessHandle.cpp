#include "../../include/ProcessHandle/ProcessHandle.h"
#include "../../include/Log/logging.h"
#include <tlhelp32.h>
#include <Psapi.h>
#include <stdio.h>
#include <Aclapi.h>
#include <Userenv.h>

#pragma comment (lib,"Psapi.lib")
#pragma comment (lib,"Userenv.lib")

DWORD ProcessHandle::GetProcessIDByName(LPCSTR pName)
{
	DWORD PID = 0;
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		printf("ProcessHandle:GetProcessIDByName 调用CreateToolhelp32Snapshot 失败！ \n");
		return 0;
	}

	PROCESSENTRY32 pe32 = { sizeof(pe32) };


	if (!Process32First(hSnapshot, &pe32))
	{
		printf("ProcessHandle:GetProcessIDByName 调用Process32First 失败，错误码:%d！\n", ::GetLastError());
		::CloseHandle(hSnapshot);
		return 0;
	}

	do
	{
		if (0 == _stricmp(pe32.szExeFile, pName))
		{
			PID = pe32.th32ProcessID;
			break;
		}

	} while (Process32Next(hSnapshot, &pe32));

	::CloseHandle(hSnapshot);

	return PID;
}

bool ProcessHandle::CheckProcessRunByPID(DWORD dwProcessID)
{
	bool bRet = false;
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		printf("ProcessHandle:GetProcessIDByName 调用CreateToolhelp32Snapshot 失败！ \n");
		return bRet;
	}

	PROCESSENTRY32 pe32 = { sizeof(pe32) };


	if (!Process32First(hSnapshot, &pe32))
	{
		printf("ProcessHandle:GetProcessIDByName 调用Process32First 失败，错误码:%d！\n", ::GetLastError());
		::CloseHandle(hSnapshot);
		return bRet;
	}

	do
	{
		if (dwProcessID == pe32.th32ProcessID)
		{
			bRet = true;
			break;
		}

	} while (Process32Next(hSnapshot, &pe32));

	::CloseHandle(hSnapshot);

	return bRet;
}

std::list<DWORD> ProcessHandle::GetProcessIDListByName(LPCSTR pProcName)
{
	std::list<DWORD> ListPID;
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		printf("ProcessHandle:GetProcessIDByName 调用CreateToolhelp32Snapshot 失败！ \n");
		return ListPID;
	}

	PROCESSENTRY32 pe32 = { sizeof(pe32) };


	if (!Process32First(hSnapshot, &pe32))
	{
		printf("ProcessHandle:GetProcessIDByName 调用Process32First 失败，错误码:%d！\n", ::GetLastError());
		::CloseHandle(hSnapshot);
		return ListPID;
	}

	do
	{
		if (0 == _stricmp(pe32.szExeFile, pProcName))
		{
			ListPID.push_back(pe32.th32ProcessID);
		}
	} while (Process32Next(hSnapshot, &pe32));

	::CloseHandle(hSnapshot);

	return ListPID;
}

void ProcessHandle::GetProcessPathByPID(DWORD dwProcessID, LPSTR buffer)
{
	TCHAR Filename[MAX_PATH];
	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
	if (hProcess == NULL) return;
	HMODULE hModule;
	DWORD cbNeeded;
	if (EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded))
	{
		if (GetModuleFileNameEx(hProcess, hModule, Filename, MAX_PATH))
		{
			RtlMoveMemory((void*)buffer, Filename, sizeof(TCHAR)*MAX_PATH);
		}
	}
	else
	{
		DWORD size = MAX_PATH;
		if (QueryFullProcessImageName(hProcess, 0, Filename, &size))	//vista 之后引入跨X32和X64
		{
			RtlMoveMemory((void*)buffer, Filename, sizeof(TCHAR)*MAX_PATH);
		}
	}
	::CloseHandle(hProcess);
}

void ProcessHandle::GetProcessPathByName(LPCSTR pName, LPSTR buffer)
{
	GetProcessPathByPID(GetProcessIDByName(pName), buffer);
}

void ProcessHandle::GetProcessDirByName(LPCSTR pName, LPSTR buffer)
{
	char strProcessPath[MAX_PATH] = {0};
	char strDrive[MAX_PATH] = { 0 };
	char strDir[MAX_PATH] = { 0 };
	buffer[0] = 0;
	GetProcessPathByPID(GetProcessIDByName(pName), strProcessPath);
	if (strlen(strProcessPath))
	{
		_splitpath_s(strProcessPath, strDrive,MAX_PATH, strDir,MAX_PATH, NULL,0, NULL,0);
		sprintf_s(buffer, MAX_PATH, "%s%s", strDrive, strDir);
	}
}

void ProcessHandle::GetProcessDirListByName(LPCSTR pProcName, std::map<DWORD, std::string> &DirList)
{
	char strProcessPath[MAX_PATH] = { 0 };
	char strDrive[MAX_PATH] = { 0 };
	char strDir[MAX_PATH] = { 0 };

	DirList.clear();
	std::list<DWORD> listPID = GetProcessIDListByName(pProcName);
	for (DWORD dwPID : listPID)
	{
		strProcessPath[0] = 0;
		GetProcessPathByPID(dwPID, strProcessPath);
		if (strlen(strProcessPath))
		{
			_splitpath_s(strProcessPath, strDrive, MAX_PATH, strDir, MAX_PATH, NULL, 0, NULL, 0);
			DirList[dwPID] = std::string(strDrive).append(strDir);
		}
	}
}

HANDLE ProcessHandle::GetProcessHandleByPID(DWORD dwProcessID)
{
	return ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
}

HANDLE ProcessHandle::GetProcessHandleByName(LPCSTR pName)
{
	DWORD dwPID = GetProcessIDByName(pName);
	if (0 == dwPID)
	{
		loge() << "get process :" << pName << " PID failed! error code:" << ::GetLastError();
		return INVALID_HANDLE_VALUE;
	}
	return GetProcessHandleByPID(dwPID);
}

bool ProcessHandle::TerminateProcess(LPCSTR pName)
{
	HANDLE hProcess = GetProcessHandleByName(pName);	//获取进程句柄
	if (INVALID_HANDLE_VALUE == hProcess)
	{
		printf("get process %s handle failed!", pName);
		return false;
	}

	if (false == ::TerminateProcess(hProcess, 0))		//终止进程
	{
		printf("TerminateProcess failed! process name:%s,error code:%d", pName,::GetLastError());
		::CloseHandle(hProcess);
		return false;
	}

	::CloseHandle(hProcess);

	//等待程序关闭结束
	do
	{
		::Sleep(1000);
	} while (GetProcessIDByName(pName));

	return true;
}

bool ProcessHandle::TerminateProcByPID(DWORD dwPID)
{
	HANDLE hProcess = GetProcessHandleByPID(dwPID);
	if (INVALID_HANDLE_VALUE == hProcess)
	{
		printf("get process %s handle failed!", dwPID);
		return false;
	}

	if (false == ::TerminateProcess(hProcess, 0))		//终止进程
	{
		printf("TerminateProcess failed! process PID:%s,error code:%d", dwPID, ::GetLastError());
		::CloseHandle(hProcess);
		return false;
	}

	::CloseHandle(hProcess);

	int iRepeatCount = 5;

	//等待程序关闭结束
	do
	{
		iRepeatCount--;
		::Sleep(2000);
	} while (CheckProcessRunByPID(dwPID) && iRepeatCount);

	return true;
}

bool ProcessHandle::NTTerminateProcess(LPCSTR lpName)
{
	typedef DWORD(CALLBACK* NTTERMINATEPROCESS)(HANDLE, UINT);
	NTTERMINATEPROCESS fcNtTerminateProcess;
	HMODULE hNtdll = LoadLibrary("ntdll.dll");

	if (!hNtdll)
	{
		printf("LoadLibrary( NTDLL.DLL ) Error:%d\n", ::GetLastError());
		return false;
	}

	fcNtTerminateProcess = (NTTERMINATEPROCESS)::GetProcAddress(hNtdll, "NtTerminateProcess");	//从ntdll.dll里获取函数

	/*
	HANDLE hToken;
	if (FALSE == ::OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))	//获取当前程序令牌
	{
	printf("Open current process Token failed!");
	return false;
	}

	if (FALSE == SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))			//启用Debug Privilege
	return false;
	*/

	DWORD dwPID = GetProcessIDByName(lpName);
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (!hProcess)
	{
		printf("获取目标进程:%s的句柄失败！", lpName);
		return false;
	}

	NTSTATUS status = fcNtTerminateProcess(hProcess, 1);
	if (0 != status)
	{
		printf("结束进程失败,返回的状态值为:%d" ,status);
		return false;
	}
	::CloseHandle(hProcess);

	//等待程序关闭结束
	do 
	{
		::Sleep(1000);
	} while (GetProcessIDByName(lpName));

	return true;
}

BOOL ProcessHandle::SetPrivilege(HANDLE hToken, LPCTSTR lpPrivilegeName, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,            // lookup privilege on local system
		lpPrivilegeName,   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		printf("LookupPrivilegeValue error: %u\n", ::GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		printf("AdjustTokenPrivileges error: %u\n", ::GetLastError());
		return FALSE;
	}

	if (::GetLastError() == ERROR_NOT_ALL_ASSIGNED)

	{
		printf("The token does not have the %s privilege. \n", lpPrivilegeName);
		return FALSE;
	}

	return TRUE;

}

bool ProcessHandle::CreateProcess(LPCSTR strAppName, LPSTR strCmdline, LPCSTR strCurDir)
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.lpDesktop = TEXT("WinSta0\\Default");
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	if (false == ::CreateProcess(strAppName, strCmdline, NULL, NULL, false, 0, NULL, strCurDir, &si, &pi))
	{
		printf("create process %s failed! error code:%d", strAppName, ::GetLastError());
		return false;
	}
	else
	{
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
	}
	return true;
}

bool ProcessHandle::CreateProcessByCmd(LPCSTR strAppName, LPSTR strCmdline, LPCSTR strCurDir)
{
	WinExec(strAppName, SW_SHOWNORMAL);
	return true;
}

BOOL ProcessHandle::CreateProcessTUser(LPCSTR strAppName, LPSTR strCmdline, LPCSTR strCurDir)
{
	if (!strAppName)
	{
		return   FALSE;
	}

	HANDLE hLocalProcToken;
	if (FALSE == ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ALL_ACCESS,
		&hLocalProcToken))	//获取当前程序令牌
	{
		printf("Open current process Token failed!");
		return FALSE;
	}

	HANDLE	 hLocalTokenDup;
	if (FALSE == ::DuplicateTokenEx(hLocalProcToken,
		MAXIMUM_ALLOWED,
		NULL,
		SecurityIdentification,
		TokenPrimary,
		&hLocalTokenDup))//创建模拟令牌   
	{
		loge() << "创建模拟令牌失败！错误码：" << ::GetLastError();
		::CloseHandle(hLocalProcToken);
		return FALSE;
	}

	// 创建一个低权限的SID
	SID_IDENTIFIER_AUTHORITY MLAuthority = SECURITY_MANDATORY_LABEL_AUTHORITY;
	PSID pIntegritySid = NULL;
	if (FALSE == AllocateAndInitializeSid(&MLAuthority, 1, SECURITY_MANDATORY_MEDIUM_RID,
		0, 0, 0, 0, 0, 0, 0, &pIntegritySid))
	{
		loge() << "create low privilege sid failed! error code:" << ::GetLastError();
		::CloseHandle(hLocalProcToken);
		::CloseHandle(hLocalTokenDup);
		return FALSE;
	}

	// 设置这个低权限SID到令牌
	TOKEN_MANDATORY_LABEL tml;
	tml.Label.Attributes = SE_GROUP_INTEGRITY;
	tml.Label.Sid = pIntegritySid;
	if (FALSE == ::SetTokenInformation(hLocalTokenDup, 
		TokenIntegrityLevel, 
		&tml,
		(sizeof(tml) + GetLengthSid(pIntegritySid))))
	{
		loge() << "set token information failed! error code:" << ::GetLastError();
		::FreeSid(pIntegritySid);
		::CloseHandle(hLocalProcToken);
		::CloseHandle(hLocalTokenDup);
		return FALSE;
	}

	STARTUPINFO   si;
	PROCESS_INFORMATION   pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = TEXT("winsta0\\default");

	/*
	LPVOID pEnv = NULL;
	if (FALSE == CreateEnvironmentBlock(&pEnv, hLocalTokenDup, FALSE))
	{
		loge() << "create environment block failed! error code:" << ::GetLastError();
		::FreeSid(pIntegritySid);
		::CloseHandle(hLocalProcToken);
		::CloseHandle(hLocalTokenDup);
		return FALSE;
	}*/

	DWORD dwCreationFlag = NORMAL_PRIORITY_CLASS;
	BOOL bRet = ::CreateProcessAsUser(hLocalTokenDup,		// hToken
		strAppName,									//lpApplicationName
		NULL,										//lpCommandLine
		NULL,										//lpProcessAttributes 
		NULL,										//lpThreadAttributes
		FALSE,										//bInheritHandles
		dwCreationFlag,								//dwCreationFlags
		NULL,										//lpEnvironment
		strCurDir,									//lpCurrentDirectory
		&si,										//lpStartupInfo
		&pi);									//lpProcessInformation

	if (bRet)
	{
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
		logm()<<"Create Process Success !";
	}
	else
	{
		loge() << "CreateProcessAsUser false! error code:" << ::GetLastError() << " app name:" << strAppName;
	}

	::FreeSid(pIntegritySid);
	::CloseHandle(hLocalProcToken);
	::CloseHandle(hLocalTokenDup);

	//::DestroyEnvironmentBlock(pEnv);

	return   bRet;
}

BOOL ProcessHandle::CreateProcessTKernel(LPCSTR strAppName, LPSTR strCmdline, LPCSTR strCurDir)
{
	if (!strAppName)
	{
		return   FALSE;
	}
	HANDLE   hToken;
	TCHAR	lpTokenName[128] = "EXPLORER.EXE";

	if (!GetTokenByName(lpTokenName, hToken))
	{
		loge()<<"get EXPLORER.EXE token failed!";
		return   FALSE;
	}
	STARTUPINFO   si;
	PROCESS_INFORMATION   pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = TEXT("winsta0\\default");

	BOOL   bResult = ::CreateProcessAsUser(hToken, 
		strAppName,
		NULL,
		NULL, 
		NULL,
		FALSE,
		NORMAL_PRIORITY_CLASS, 
		NULL, 
		strCurDir,
		&si,
		&pi);

	::CloseHandle(hToken);
	if (bResult)
	{
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
		logm()<<"Create Process: \""<<strAppName<<"\" sucessed !";
	}
	else
	{
		loge() << "Create Process: \"" << strAppName << "\" false! error code:" << ::GetLastError();
	}
	return   bResult;
}

BOOL ProcessHandle::GetTokenByName(LPTSTR lpName, HANDLE &hToken)
{
	if (!lpName)
	{
		printf("process name is NULL! \n");
		return   FALSE;
	}

	MHandle hProcess = GetProcessHandleByName(lpName);
	if(INVALID_HANDLE_VALUE == hProcess)
	{
		loge() << "Get process handle faild! error code:" << ::GetLastError();
		return FALSE;
	}

	if (FALSE == ::OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken))
	{
		loge() << "获取process token failed! error code:" << ::GetLastError();
		return FALSE;
	}

	return   TRUE;
}

void enumProcessPath()
{
	DWORD processid[1024], needed, processcount, i;
	HANDLE hProcess;
	HMODULE hModule;
	char path[MAX_PATH] = "";
	printf("ShowProcessPath with [Process Status API]/n");
	EnumProcesses(processid, sizeof(processid), &needed);
	processcount = needed / sizeof(DWORD);
	for (i = 0; i < processcount; i++)
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, processid[i]);
		if (hProcess)
		{
			EnumProcessModules(hProcess, &hModule, sizeof(hModule), &needed);
			GetModuleFileNameEx(hProcess, hModule, path, sizeof(path));
			GetShortPathName(path, path, 256);
			printf("%s --- %d/n", path, processid[i]);
		}
		else
		{
			printf("Failed!!!/n");
		}
	}
	CloseHandle(hProcess);
	CloseHandle(hModule);
	printf("/nProcess Count:%d/n/n", processcount);

}
