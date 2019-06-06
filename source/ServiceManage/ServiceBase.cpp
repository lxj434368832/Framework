#include "../../include/ServiceManage/ServiceBase.h"  
#include "../../include/ServiceManage/ServiceOperate.h"
#include <cassert>  
#include <stdio.h>
#include <stdarg.h>
#include <Aclapi.h>

ServiceBase* ServiceBase::m_service = nullptr;

ServiceBase::ServiceBase(const CString& name,
	const CString& displayName,
	DWORD dwStartType,
	DWORD dwErrCtrlType,
	DWORD dwAcceptedCmds,
	const CString& depends,
	const CString& account,
	const CString& password)
	: m_name(name),
	m_displayName(displayName),
	m_dwStartType(dwStartType),
	m_dwErrorCtrlType(dwErrCtrlType),
	m_depends(depends),
	m_account(account),
	m_password(password),
	m_svcStatusHandle(nullptr)
{
	m_hasDepends = !m_depends.IsEmpty();
	m_hasAcc = !m_account.IsEmpty();
	m_hasPass = !m_password.IsEmpty();
	m_svcStatus.dwControlsAccepted = dwAcceptedCmds;
	m_svcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_svcStatus.dwCurrentState = SERVICE_START_PENDING;
	m_svcStatus.dwWin32ExitCode = NO_ERROR;
	m_svcStatus.dwServiceSpecificExitCode = 0;
	m_svcStatus.dwCheckPoint = 0;
	m_svcStatus.dwWaitHint = 0;
	m_service = nullptr;
}

int ServiceBase::ProcessMain(int argc, _TCHAR* argv[])
{
	//带有参数的调用  
	if (argc > 1 && ((*argv[1] == '-') || (*argv[1] == '/')))
	{
		if (_stricmp(argv[1] + 1, _T("install")) == 0 || _stricmp(argv[1] + 1, "i") == 0) 
		{//安装服务  
			_tprintf(_T("ServiceBase:Installing service \"%s\"\n"),m_name.GetBuffer());

			if (!ServiceOperate::Install(m_name.GetBuffer(), GetStartType(), GetErrorControlType()))
			{
				_tprintf(_T("ServiceBase:Couldn't install service \"%s\"\n"),m_name.GetBuffer());
				return -1;
			}

			_tprintf(_T("ServiceBase:Service \"%s\" installed\n"), m_name.GetBuffer());
			return 0;
		}

		else if (_stricmp(argv[1] + 1, _T("remove")) == 0 || _stricmp(argv[1] + 1, "r") == 0) 
		{//卸载服务  
			_tprintf(_T("ServiceBase:Uninstalling service \"%s\"\n"),m_name.GetBuffer());
			if (!ServiceOperate::Uninstall(m_name.GetBuffer()))
			{
				_tprintf(_T("ServiceBase:Couldn't uninstall service: \"%s\"\n"),m_name.GetBuffer());
				return -1;
			}

			_tprintf(_T("ServiceBase:Service \"%s\" uninstalled\n"), m_name.GetBuffer());
			return 0;
		}
		else if (_stricmp(argv[1] + 1, _T("start")) == 0)
		{//开始服务
			if (!ServiceOperate::Start(m_name.GetBuffer()))
			{
				_tprintf(_T("ServiceBase:Couldn't start service: %s\n"), m_name.GetBuffer());
				return -1;
			}
			return 0;
		}
		else if (_stricmp(argv[1] + 1, _T("stop")) == 0)
		{//停止服务
			if (!ServiceOperate::Stop(m_name.GetBuffer()))
			{
				_tprintf(_T("ServiceBase:Couldn't stop service: %s\n"), m_name.GetBuffer());
				return -1;
			}
			return 0;
		}
		else if (_stricmp(argv[1] + 1, _T("debug")) == 0 || _stricmp(argv[1] + 1, "d") == 0)//以调试方式运行
		{
			this->DebugRun();
			return 0;
		}
		else if (_stricmp(argv[1] + 1, _T("console")) == 0 || _stricmp(argv[1] + 1, "c") == 0) {//以控制台的方式运行  
			_tprintf(_T("ServiceBase:console running\n"));
			this->CmdRun();
			return 0;
		}
		else
		{
			DisplayMainUsage();
			return -1;
		}
	}
	else
	{//以服务的方式运行
		OutputDebugString(_T("ServiceBase:start service"));
		this->ServiceRun();
		return 0;
	}

}

void ServiceBase::SetStatus(DWORD dwState, DWORD dwErrCode, DWORD dwWait)
{
	m_svcStatus.dwCurrentState = dwState;
	m_svcStatus.dwWin32ExitCode = dwErrCode;
	m_svcStatus.dwWaitHint = dwWait;
	::SetServiceStatus(m_svcStatusHandle, &m_svcStatus);
}

void ServiceBase::WriteToEventLog(const CString& msg, WORD type)
{
	HANDLE hSource = RegisterEventSource(nullptr, m_name);
	if (hSource)
	{
		const TCHAR* msgData[2] = { m_name, msg };
		ReportEvent(hSource, type, 0, 0, nullptr, 2, 0, msgData, nullptr);
		DeregisterEventSource(hSource);
	}
}

void ServiceBase::LogEvent(WORD type, LPCTSTR pszFormat, ...)
{
	TCHAR    chMsg[256];
	HANDLE  hEventSource;
	va_list pArg;

	va_start(pArg, pszFormat);
	_vstprintf_s(chMsg, pszFormat, pArg);
	va_end(pArg);

	LPCTSTR  lpszStrings[2] = { m_name.GetBuffer(), chMsg };

	hEventSource = RegisterEventSource(NULL, m_name);
	if (hEventSource != NULL)
	{
		ReportEvent(hEventSource,		  // handle of event source
					type,				  // event type
					0,                    // event category
					0,                    // event ID
					NULL,                 // current user's SID
					2,                    // strings in lpszStrings
					0,                    // no bytes of raw data
					lpszStrings,		  // array of error strings
					NULL);                // no raw data

		DeregisterEventSource(hEventSource);
	}
}

bool ServiceBase::ServiceRun()
{
	m_service = this;
	TCHAR* svcName = const_cast<CString&>(m_service->GetName()).GetBuffer();

	SERVICE_TABLE_ENTRY tableEntry[] =
	{
		{ svcName, SvcMain },
		{ nullptr, nullptr }
	};

	return ::StartServiceCtrlDispatcher(tableEntry) == TRUE;
}

// static  
void WINAPI ServiceBase::SvcMain(DWORD argc, TCHAR* argv[])
{
	assert(m_service);
	m_service->m_svcStatusHandle = ::RegisterServiceCtrlHandlerEx(m_service->GetName(), ServiceCtrlHandler, NULL);
	if (!m_service->m_svcStatusHandle)
	{
		m_service->WriteToEventLog(_T("Can't set service control handler"), EVENTLOG_ERROR_TYPE);
		return;
	}

	m_service->Start(argc, argv);
}

// static  
DWORD WINAPI ServiceBase::ServiceCtrlHandler(DWORD ctrlCode, DWORD evtType, void* evtData, void* /*context*/)
{
	switch (ctrlCode)
	{
	case SERVICE_CONTROL_STOP:
		m_service->Stop();
		break;

	case SERVICE_CONTROL_PAUSE:
		m_service->Pause();
		break;

	case SERVICE_CONTROL_CONTINUE:
		m_service->Continue();
		break;

	case SERVICE_CONTROL_SHUTDOWN:
		m_service->Shutdown();
		break;

	case SERVICE_CONTROL_SESSIONCHANGE:
		m_service->OnSessionChange(evtType, reinterpret_cast<WTSSESSION_NOTIFICATION*>(evtData));
		break;
	default:
		break;
	}

	return 0;
}

void ServiceBase::DisplayMainUsage()
{
	printf(_T("\r\n========================================\r\n"));
	printf("-install /-i      to install the service\n");
	printf("-remove /-r       to remove the service\n");
	printf("-debug /-d		  to debug the service\n");
	printf("-cmd /-c		  to run as a console app for debugging\n");
	printf("-start 			  to start the service\n");
	printf("-stop             to stop the service\n");
	printf(_T("\r\n========================================\r\n"));
}

bool ServiceBase::CmdRun()
{
	//Todo：控制台运行处理 调用OnStart和OnStop  

	TCHAR cinCmd[128];
	bool bStart = false;
	bool bServiceInstall = false;

	while (1)//控制台运行主线程不能退出   
	{
		_tprintf(_T("->input cmd,? help\r\n"));

		_tscanf_s(_T("%s"), cinCmd, 128);		//接收控制台输入
		if (_tcsncmp(cinCmd, _T("?"), 1) == 0)
		{
			_tprintf(_T("\r\n========================================\r\n"));
			_tprintf(_T("\"?\"     -show cmd help\r\n"));
			_tprintf(_T("\"start\" -start service\r\n"));
			_tprintf(_T("\"stop\"  -stop service\r\n"));
			_tprintf(_T("\"exit\"  -exit service\r\n"));
			_tprintf(_T("========================================\r\n"));
		}
		else if (_tcsncmp(cinCmd, _T("start"), 5) == 0)
		{
			if (!bStart)
			{
				OnStart(0, NULL);
				_tprintf(_T("-> start service\r\n"));
			}
			bStart = true;
		}
		else if (_tcsncmp(cinCmd, _T("stop"), 4) == 0)
		{
			if (bStart)
			{
				OnStop();
				_tprintf(_T("-> stop service\r\n"));
			}

			bStart = false;
		}
		else if (_tcsncmp(cinCmd, _T("exit"), 4) == 0)
		{
			_tprintf(_T("-> exit service\r\n"));

			break;
		}
		else
		{
			_tprintf(_T("invalid cmd\r\n"));
		}
	}

	if (bStart)
		OnStop();

	return true;
}

bool ServiceBase::DebugRun()
{
	m_service = this;
	::SetConsoleCtrlHandler(ControlCtrlHandler, TRUE);

	OnStart(0, NULL);

	OnContinueRun();
	return true;
}

BOOL WINAPI ServiceBase::ControlCtrlHandler(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_BREAK_EVENT:  // use Ctrl+C or Ctrl+Break to simulate
	case CTRL_C_EVENT:      // SERVICE_CONTROL_STOP in debug mode
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		_tprintf(TEXT("Stopping %s.\n"), m_service->m_name.GetBuffer());
		m_service->OnStop();
		return TRUE;
	default:
		break;

	}
	return FALSE;
}

void ServiceBase::Start(DWORD argc, TCHAR* argv[])
{
	SetStatus(SERVICE_START_PENDING);
	OnStart(argc, argv);
	SetStatus(SERVICE_RUNNING);

	OnContinueRun();
}

void ServiceBase::Stop()
{
	SetStatus(SERVICE_STOP_PENDING);
	OnStop();
	SetStatus(SERVICE_STOPPED);
}

void ServiceBase::Pause()
{
	SetStatus(SERVICE_PAUSE_PENDING);
	OnPause();
	SetStatus(SERVICE_PAUSED);
}

void ServiceBase::Continue()
{
	SetStatus(SERVICE_CONTINUE_PENDING);
	OnContinue();
	SetStatus(SERVICE_RUNNING);
}

void ServiceBase::Shutdown()
{
	OnShutdown();
	SetStatus(SERVICE_STOPPED);
}