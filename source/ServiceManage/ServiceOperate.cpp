#include "../../include/ServiceManage/ServiceOperate.h"  
#include <memory>

namespace
{
	class ServiceHandle
	{
	public:
		ServiceHandle() :m_handle(NULL){}
		ServiceHandle(SC_HANDLE handle) : m_handle(handle) {}

		~ServiceHandle()
		{
			if (m_handle)
			{
				::CloseServiceHandle(m_handle);
			}
		}

		operator SC_HANDLE()
		{
			return m_handle;
		}

	private:
		SC_HANDLE m_handle;
	};
}

//static  
bool ServiceOperate::Install(LPCTSTR lpServiceName, DWORD dwStartType, DWORD dwErrorControlType)
{
	CString escapedPath;
	TCHAR* modulePath = escapedPath.GetBufferSetLength(MAX_PATH);

	if (::GetModuleFileName(nullptr, modulePath, MAX_PATH) == 0)
	{
		_tprintf(_T("Couldn't get module file name: %d\n"), ::GetLastError());
		return false;
	}

	escapedPath.ReleaseBuffer();
	escapedPath.Remove(_T('\"'));

	escapedPath = _T('\"') + escapedPath + _T('\"');

	ServiceHandle svcControlManager = ::OpenSCManager(
		NULL,                    // local computer
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (!svcControlManager)
	{
		_tprintf(_T("couldn't open service control manager! error code: %d\n"), GetLastError());
		return false;
	}

	const CString depends;
	const CString acc;
	const CString pass;

	ServiceHandle servHandle = ::CreateService(
		svcControlManager,										// SCM database 
		lpServiceName,											// name of service 
		lpServiceName,											// service name to displ
		SERVICE_ALL_ACCESS,										// desired access 
		SERVICE_WIN32_OWN_PROCESS,								// service type 
		dwStartType,											// start type 
		dwErrorControlType,										// error control type 
		escapedPath,											// path to service's bin
		nullptr,												// no load ordering grou
		nullptr,												// no tag identifier 
		(depends.IsEmpty() ? nullptr : depends.GetString()),	// no dependencies 
		(acc.IsEmpty() ? nullptr : acc.GetString()),			// LocalSystem account 
		(pass.IsEmpty() ? nullptr : pass.GetString()));			// no password 

	if (!servHandle)
	{
		_tprintf(_T("Couldn't create service: %d\n"), ::GetLastError());
		return false;
	}

	printf("service \"%s\" install success!\n", lpServiceName);

	//设置自动重启，可选项

	SC_ACTION act[3];
	//First failure:
	act[0].Delay = 60 * 1000; // wait 60 seconds before restarting
	act[0].Type = SC_ACTION_RESTART;

	//Second failure:
	act[1].Delay = 60 * 1000;
	act[1].Type = SC_ACTION_RESTART;

	//Subsequent failure:
	act[2].Delay = 60 * 1000;
	act[2].Type = SC_ACTION_RESTART;

	SERVICE_FAILURE_ACTIONS sfa;
	sfa.lpRebootMsg = NULL;
	sfa.dwResetPeriod = 3600 * 24; // Reset the error count after 1day
	sfa.lpsaActions = act;
	sfa.lpCommand = NULL;
	sfa.cActions = 3;

	if (!ChangeServiceConfig2(servHandle, SERVICE_CONFIG_FAILURE_ACTIONS, &sfa))
		_tprintf(TEXT("setServiceRecovery 设置自动重启失败，错误码:%d,请手动设置服务恢复选项!\n"),::GetLastError());

	return true;
}

//static  
bool ServiceOperate::Uninstall(LPCTSTR lpServiceName)
{
	ServiceHandle svcControlManager = ::OpenSCManager(
		NULL,                    // local computer
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (!svcControlManager)
	{
		_tprintf(_T("Couldn't open service control manager! error code: %d\n"), GetLastError());
		return false;
	}

	ServiceHandle servHandle = ::OpenService(
		svcControlManager,       // SCM database 
		lpServiceName,          // name of service 
		SERVICE_QUERY_STATUS | SERVICE_STOP | DELETE); // need delete access 

	if (!servHandle)
	{
		_tprintf(_T("Couldn't open service \"%s\"! error code: %d\n"),lpServiceName, ::GetLastError());
		return false;
	}

	Stop(lpServiceName);

	if (!::DeleteService(servHandle))
	{
		_tprintf(_T("Failed to delete the service: %d\n"), ::GetLastError());
		return false;
	}

	printf("service \"%s\" delete success!\n", lpServiceName);
	return true;
}

BOOL ServiceOperate::IsInstalled(LPCTSTR lpServiceName)
{
	BOOL bResult = FALSE;

	//打开服务控制管理器  
	ServiceHandle hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		//打开服务  
		ServiceHandle hService = ::OpenService(hSCM, lpServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
		}
	}

	return bResult;
}

bool ServiceOperate::Start(LPCTSTR lpServiceName)
{
	ServiceHandle schSCManager = OpenSCManager(
		NULL,                    // local computer
		NULL,                    // servicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager)
	{
		printf("Couldn't open service control manager! error code: %d\n", ::GetLastError());
		return false;
	}

	ServiceHandle schService = OpenService(
		schSCManager,         // SCM database 
		lpServiceName,        // name of service 
		SERVICE_ALL_ACCESS);  // full access 

	if (NULL == schService)
	{
		printf("Couldn't open service \"%s\"! error code: %d\n", lpServiceName, ::GetLastError());
		return false;
	}

	// Check the status in case the service is not stopped. 

	SERVICE_STATUS_PROCESS ssStatus;
	DWORD dwTimeout = 10000;	//10-second time-out
	DWORD dwStartTickCount;
	DWORD dwWaitTime;
	DWORD dwBytesNeeded;

	if (!QueryServiceStatusEx(
		schService,                     // handle to service 
		SC_STATUS_PROCESS_INFO,         // information level
		(LPBYTE)&ssStatus,             // address of structure
		sizeof(SERVICE_STATUS_PROCESS), // size of structure
		&dwBytesNeeded))              // size needed if buffer is too small
	{
		printf("QueryServiceStatusEx failed (%d)\n", ::GetLastError());
		return false;
	}

	// Check if the service is already running. It would be possible 
	// to stop the service here. 

	if (ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
	{
		printf("Cannot start the service \"%s\" because it is already running\n", lpServiceName);
		return true;
	}

	// Save the tick count and initial checkpoint.
	dwStartTickCount = GetTickCount();

	// Wait for the service to stop before attempting to start it.
	while (SERVICE_STOP_PENDING == ssStatus.dwCurrentState)
	{
		// Do not wait longer than the wait hint. A good interval is 
		// one-tenth of the wait hint but not less than 1 second  
		// and not more than 10 seconds. 

		dwWaitTime = ssStatus.dwWaitHint / 10;

		if (dwWaitTime < 1000)
			dwWaitTime = 1000;
		else if (dwWaitTime > 10000)
			dwWaitTime = 10000;

		Sleep(dwWaitTime);

		// Check the status until the service is no longer stop pending. 

		if (!QueryServiceStatusEx(
			schService,						// handle to service 
			SC_STATUS_PROCESS_INFO,			// information level
			(LPBYTE)&ssStatus,				// address of structure
			sizeof(SERVICE_STATUS_PROCESS),	// size of structure
			&dwBytesNeeded))				// size needed if buffer is too small
		{
			printf("QueryServiceStatusEx failed (%d)\n", ::GetLastError());
			return false;
		}

		if (::GetTickCount() - dwStartTickCount > dwTimeout)
		{
			printf("Timeout waiting for service to stop\n");
			return false;
		}

	}

	// Attempt to start the service.
	if (false == StartService(
		schService,	// handle to service 
		0,		    // number of arguments 
		NULL))	    // no arguments 
	{
		printf("StartService failed (%d)\n", ::GetLastError());
		return false;
	}
	else  printf("Service start pending...\n");

	// Check the status until the service is no longer start pending. 
	if (false == QueryServiceStatusEx(
		schService,						// handle to service 
		SC_STATUS_PROCESS_INFO,			// info level
		(LPBYTE)&ssStatus,				// address of structure
		sizeof(SERVICE_STATUS_PROCESS),	// size of structure
		&dwBytesNeeded))				// if buffer too small
	{
		printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
		return false;
	}

	// Save the tick count and initial checkpoint.
	dwStartTickCount = GetTickCount();

	while (SERVICE_START_PENDING == ssStatus.dwCurrentState)
	{
		// Do not wait longer than the wait hint. A good interval is 
		// one-tenth the wait hint, but no less than 1 second and no 
		// more than 10 seconds. 
		dwWaitTime = ssStatus.dwWaitHint / 10;

		if (dwWaitTime < 1000)
			dwWaitTime = 1000;
		else if (dwWaitTime > 10000)
			dwWaitTime = 10000;
		::Sleep(dwWaitTime);

		// Check the status again. 
		if (!QueryServiceStatusEx(
			schService,						// handle to service 
			SC_STATUS_PROCESS_INFO,			// info level
			(LPBYTE)&ssStatus,				// address of structure
			sizeof(SERVICE_STATUS_PROCESS),	// size of structure
			&dwBytesNeeded))				// if buffer too small
		{
			printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
			break;
		}

		if (::GetTickCount() - dwStartTickCount > dwTimeout)
		{
			printf("Timeout waiting for service to start\n");
			return false;
		}

	}//end while

	// Determine whether the service is running.

	if (SERVICE_RUNNING == ssStatus.dwCurrentState)
	{
		printf("Service \"%s\" started successfully.\n" ,lpServiceName);
		return true;
	}
	else
	{
		printf("Service not started. \n");
		printf("  Current State: %d\n", ssStatus.dwCurrentState);
		printf("  Exit Code: %d\n", ssStatus.dwWin32ExitCode);
		printf("  Check Point: %d\n", ssStatus.dwCheckPoint);
		printf("  Wait Hint: %d\n", ssStatus.dwWaitHint);
		return false;
	}

}

bool ServiceOperate::Stop(LPCTSTR lpServiceName)
{
	ServiceHandle schSCManager = ::OpenSCManager(
		NULL,					// local computer
		NULL,					// ServicesActive database
		SC_MANAGER_ALL_ACCESS);	// full access rights 

	if (NULL == schSCManager)
	{
		printf("OpenSCManager failed (%d)\n", GetLastError());
		return false;
	}

	// Get a handle to the service.
	ServiceHandle schService = ::OpenService(
		schSCManager,				// SCM database 
		lpServiceName,				// name of service 
		SERVICE_STOP | SERVICE_QUERY_STATUS |
		SERVICE_ENUMERATE_DEPENDENTS);

	if (NULL == schService)
	{
		printf("OpenService failed (%d)\n", GetLastError());
		return false;
	}

	// Make sure the service is not already stopped.

	SERVICE_STATUS_PROCESS ssp;
	DWORD dwStartTime;
	DWORD dwBytesNeeded;
	DWORD dwTimeout = 30000; // 30-second time-out
	DWORD dwWaitTime;

	if (!QueryServiceStatusEx(
		schService,
		SC_STATUS_PROCESS_INFO,
		(LPBYTE)&ssp,
		sizeof(SERVICE_STATUS_PROCESS),
		&dwBytesNeeded))
	{
		printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
		return false;
	}

	if (SERVICE_STOPPED == ssp.dwCurrentState)
	{
		printf("Service \"%s\" is already stopped.\n", lpServiceName);
		return true;
	}

	dwStartTime = ::GetTickCount();
	// If a stop is pending, wait for it.
	while (SERVICE_STOP_PENDING == ssp.dwCurrentState)
	{
		printf("Service stop pending...\n");

		// Do not wait longer than the wait hint. A good interval is 
		// one-tenth of the wait hint but not less than 1 second  
		// and not more than 10 seconds. 
		dwWaitTime = ssp.dwWaitHint / 10;

		if (dwWaitTime < 1000)
			dwWaitTime = 1000;
		else if (dwWaitTime > 10000)
			dwWaitTime = 10000;
		::Sleep(dwWaitTime);

		if (!QueryServiceStatusEx(
			schService,
			SC_STATUS_PROCESS_INFO,
			(LPBYTE)&ssp,
			sizeof(SERVICE_STATUS_PROCESS),
			&dwBytesNeeded))
		{
			printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
			return false;
		}

		if (SERVICE_STOPPED == ssp.dwCurrentState)
		{
			printf("Service \"%s\" stopped successfully.\n" ,lpServiceName);
			return false;
		}

		if (::GetTickCount() - dwStartTime > dwTimeout)
		{
			printf("Service stop timed out.\n");
			return false;
		}
	}	//end while


	// If the service is running, dependencies must be stopped first.

	StopDependentServices(schSCManager, schService);

	// Send a stop code to the service.

	if (FALSE == ControlService(
		schService,
		SERVICE_CONTROL_STOP,
		(LPSERVICE_STATUS)&ssp))
	{
		printf("ControlService failed (%d)\n", GetLastError());
		return false;
	}

	// Wait for the service to stop.
	dwStartTime = ::GetTickCount();
	while (SERVICE_STOPPED != ssp.dwCurrentState)
	{
		::Sleep(ssp.dwWaitHint);
		if (!QueryServiceStatusEx(
			schService,
			SC_STATUS_PROCESS_INFO,
			(LPBYTE)&ssp,
			sizeof(SERVICE_STATUS_PROCESS),
			&dwBytesNeeded))
		{
			printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
			return false;
		}

		if (SERVICE_STOPPED == ssp.dwCurrentState)
			break;

		if (GetTickCount() - dwStartTime > dwTimeout)
		{
			printf("Wait timed out\n");
			return false;
		}
	}

	printf("Service \"%s\" stopped successfully.\n", lpServiceName);
	return true;
}

BOOL ServiceOperate::StopDependentServices(SC_HANDLE schSCManager, SC_HANDLE schService)
{
	std::unique_ptr<ENUM_SERVICE_STATUS>   lpDependencies;

	DWORD dwBytesNeeded;
	DWORD dwCount;

	DWORD dwStartTime = GetTickCount();
	DWORD dwTimeout = 30000; // 30-second time-out

	// Pass a zero-length buffer to get the required buffer size.
	if (EnumDependentServices(schService, SERVICE_ACTIVE,
		lpDependencies.get(), 0, &dwBytesNeeded, &dwCount))
	{
		// If the Enum call succeeds, then there are no dependent
		// services, so do nothing.
		return TRUE;
	}
	else
	{
		if (ERROR_MORE_DATA != GetLastError())
			return FALSE; // Unexpected error

		// Allocate a buffer for the dependencies.
		lpDependencies.reset ((LPENUM_SERVICE_STATUS)HeapAlloc(
			GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded));

		if (NULL == lpDependencies.get())
			return FALSE;


		// Enumerate the dependencies.
		ENUM_SERVICE_STATUS     ess;
		SERVICE_STATUS_PROCESS  ssp;
		ServiceHandle hDepService;

		if (!EnumDependentServices(
			schService,
			SERVICE_ACTIVE,
			lpDependencies.get(),
			dwBytesNeeded,
			&dwBytesNeeded,
			&dwCount))
			return FALSE;

		for (DWORD i = 0; i < dwCount; i++)
		{
			ess = *(lpDependencies.get() + i);
			//Open the service.
			hDepService = OpenService(
				schSCManager,
				ess.lpServiceName,
				SERVICE_STOP | SERVICE_QUERY_STATUS);
			if (!hDepService)
				return FALSE;

			// Send a stop code.
			if (!ControlService(
				hDepService,
				SERVICE_CONTROL_STOP,
				(LPSERVICE_STATUS)&ssp))
				return FALSE;

			//Wait for the service to stop.
			while (SERVICE_STOPPED != ssp.dwCurrentState)
			{
				::Sleep(ssp.dwWaitHint);
				if (!QueryServiceStatusEx(
					hDepService,
					SC_STATUS_PROCESS_INFO,
					(LPBYTE)&ssp,
					sizeof(SERVICE_STATUS_PROCESS),
					&dwBytesNeeded))
					return FALSE;

				if (SERVICE_STOPPED == ssp.dwCurrentState)
					break;

				if (::GetTickCount() - dwStartTime > dwTimeout)
					return FALSE;
			}

		}// end for

		return TRUE;
	}//end else
}

void ServiceOperate::ReconfigureService(LPSTR lpServiceName, LPSTR lpDesc)
{
	SC_HANDLE schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (schSCManager != NULL)
	{
		// Need to acquire database lock before reconfiguring. 
		SC_LOCK sclLock = LockServiceDatabase(schSCManager);
		if (sclLock != NULL)
		{
			// Open a handle to the service. 
			SC_HANDLE schService = OpenService(
				schSCManager,           // SCManager database 
				lpServiceName,          // name of service 
				SERVICE_CHANGE_CONFIG); // need CHANGE access 

			if (schService != NULL)
			{
				SERVICE_DESCRIPTION sdBuf;
				sdBuf.lpDescription = lpDesc;
				if (ChangeServiceConfig2(
					schService, SERVICE_CONFIG_DESCRIPTION, &sdBuf))
				{
					//MessageBox(NULL,"Change SUCCESS","",MB_SERVICE_NOTIFICATION); 
				}
				CloseServiceHandle(schService);
			}
			UnlockServiceDatabase(sclLock);
		}
		CloseServiceHandle(schSCManager);
	}
}
