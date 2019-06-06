#include "CService.h"
#include "../../include/ServiceManage/IService.h"

//������CService��ʵ��
class ConcreteService : public IService
{
	bool Start() { printf("��ʼ����"); return true; }
	void Stop() { printf("ֹͣ����"); }
};


//������ʱ���õĽӿڣ��˽ӿڲ���������������������һֱ��ʾ�����У�
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

//����ر�ʱ���õĽӿڣ��˽ӿڲ���������������������һֱ��ʾ�ر��У�  
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
