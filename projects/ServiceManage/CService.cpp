#include "CService.h"
#include "../../include/ServiceManage/IService.h"

//以下是CService的实现
class ConcreteService : public IService
{
	bool Start() { printf("开始服务！"); return true; }
	void Stop() { printf("停止服务！"); }
};


//服务开启时调用的接口（此接口不能阻塞，如果阻塞服务会一直显示开启中）
void CService::OnStart(DWORD argc, TCHAR* argv[])
{
	argc;	argv;
	OutputDebugString(_T("CService:Application start running!!!\n"));

	m_interface = new ConcreteService(); 
	m_interface->Start();
}

void CService::OnContinueRun()
{
	m_bRun = true;
	//MSG   msg;
	//while (m_bRun && ::GetMessage(&msg, NULL, 0, 0))
	//{
	//	::TranslateMessage(&msg);
	//}
	while (m_bRun)
	{
		::Sleep(1000);
	}
}

//服务关闭时调用的接口（此接口不能阻塞，如果阻塞服务会一直显示关闭中）  
void CService::OnStop()
{
	OutputDebugString(_T("CService:Application end\n"));

	m_bRun = false;
	if (m_interface)
	{
		m_interface->Stop();
		delete m_interface;
		m_interface = nullptr;
	}
	else
		printf("%s:%d m_interface is NULL\n",__FUNCTION__,__LINE__);
}
