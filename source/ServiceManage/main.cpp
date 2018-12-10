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

//����̳л���������
class CService :public ServiceBase
{
public:
	CService(const CString&ServiceName);

private:
	//������ʱ���õĽӿڣ��˽ӿڲ���������������������һֱ��ʾ�����У�  
	void OnStart(DWORD argc, TCHAR* argv[]);

	//����ر�ʱ���õĽӿڣ��˽ӿڲ���������������������һֱ��ʾ�ر��У�  
	void OnStop();

private:
	IService *m_interface = NULL;
};


int _tmain(int argc, _TCHAR* argv[])
{
	CService server(SERVICE_NAME);
	return server.ProcessMain(argc, argv);
}


//������CService��ʵ��
class ConcreteService : public IService
{
	void Start(DWORD argc, TCHAR* argv[]){ printf("��ʼ����"); }
	void Stop(){ printf("ֹͣ����"); }
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

//������ʱ���õĽӿڣ��˽ӿڲ���������������������һֱ��ʾ�����У�
void CService::OnStart(DWORD argc, TCHAR* argv[])
{
	OutputDebugString(_T("CService:Application start running!!!\n"));
	//TO_DO: start
	m_interface = new ConcreteService();
	m_interface->Start(argc, argv);
}

//����ر�ʱ���õĽӿڣ��˽ӿڲ���������������������һֱ��ʾ�ر��У�  
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


/* ����ֱ����CServiceһ��ֱ�Ӽ̳�ServiceBaseʵ��OnStart��OnStop

//����̳л���������
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
//������ʱ���õĽӿڣ��˽ӿڲ���������������������һֱ��ʾ�����У�
void OnStart(DWORD argc, TCHAR* argv[])
{
OutputDebugString(_T("TestService:Application start running!!!\n"));
//TO_DO: start
}

//����ر�ʱ���õĽӿڣ��˽ӿڲ���������������������һֱ��ʾ�ر��У�
void OnStop()
{
OutputDebugString(_T("TestService:Application end\n"));
//TO_DO: stop

}

private:
};*/

