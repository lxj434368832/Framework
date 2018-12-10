#include <windows.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include "../../include/ServiceManage/ServiceBase.h"
#include "../../include/ServiceManage/IService.h"

#ifdef UPDATE_DEAMON
#define SERVICE_NAME _T("Ambition AutoUpdate Deamon")
#else
#define SERVICE_NAME _T("Ambition Auto Update")
#endif // UPDATE_DEAMON

//此类继承基础服务类
class CService :public ServiceBase
{
public:
	CService(const CString&ServiceName);

private:
	//服务开启时调用的接口（此接口不能阻塞，如果阻塞服务会一直显示开启中）  
	void OnStart(DWORD argc, TCHAR* argv[]);

	//服务关闭时调用的接口（此接口不能阻塞，如果阻塞服务会一直显示关闭中）  
	void OnStop();

private:
	IService *m_interface = NULL;
};


int _tmain(int argc, _TCHAR* argv[])
{
	CService server(SERVICE_NAME);
	return server.ProcessMain(argc, argv);
}


//以下是CService的实现
class ConcreteService : public IService
{
	void Start(DWORD argc, TCHAR* argv[]){ printf("开始服务！"); }
	void Stop(){ printf("停止服务！"); }
};

CService::CService(const CString& ServiceName)
	: ServiceBase(ServiceName,
	ServiceName,
	SERVICE_AUTO_START,			//start type
	SERVICE_ERROR_NORMAL,		//error control type
	SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN	//accept cmd
	)
{

}

//服务开启时调用的接口（此接口不能阻塞，如果阻塞服务会一直显示开启中）
void CService::OnStart(DWORD argc, TCHAR* argv[])
{
	OutputDebugString(_T("CService:Application start running!!!\n"));
	//TO_DO: start
	m_interface = new ConcreteService();
	m_interface->Start(argc, argv);
}

//服务关闭时调用的接口（此接口不能阻塞，如果阻塞服务会一直显示关闭中）  
void CService::OnStop()
{
	OutputDebugString(_T("CService:Application end\n"));
	//TO_DO: stop
	if (m_interface)
	{
		m_interface->Stop();
		delete m_interface;
		m_interface = NULL;
	}
	else
		printf("%s:%d m_interface is NULL\n", __FUNCTION__, __LINE__);
}


/* 或者直接像CService一样直接继承ServiceBase实现OnStart和OnStop

//此类继承基础服务类
class CService :public ServiceBase
{
public:
CService(const CString&ServiceName)
:ServiceBase(ServiceName,
ServiceName,
SERVICE_DEMAND_START,
SERVICE_ERROR_NORMAL,
SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN
){}

private:
//服务开启时调用的接口（此接口不能阻塞，如果阻塞服务会一直显示开启中）
void OnStart(DWORD argc, TCHAR* argv[])
{
OutputDebugString(_T("TestService:Application start running!!!\n"));
//TO_DO: start
}

//服务关闭时调用的接口（此接口不能阻塞，如果阻塞服务会一直显示关闭中）
void OnStop()
{
OutputDebugString(_T("TestService:Application end\n"));
//TO_DO: stop

}

private:
};*/

