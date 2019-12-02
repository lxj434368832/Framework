#pragma once
/************************************************************************
* function�������ܾ����ʵ����
* author :	������
* datetime��2018-04-03
* company:  ���̽ݿƼ��ɷ����޹�˾
/************************************************************************/

#include "../../include/ServiceManage/ServiceBase.h"

class IService;

//����̳л���������
class CService :public ServiceBase
{
public:
	CService(const CString&ServiceName)
		:ServiceBase(ServiceName,
		ServiceName,
		SERVICE_AUTO_START,			//start type
		SERVICE_ERROR_NORMAL,		//error control type
		SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN	//accept cmd
		){}

private:
	//������ʱ���õĽӿڣ��˽ӿڲ���������������������һֱ��ʾ�����У�  
	void OnStart(DWORD argc, TCHAR* argv[]) override;

	//�˲���������
	void OnContinueRun() override;

	//����ر�ʱ���õĽӿڣ��˽ӿڲ���������������������һֱ��ʾ�ر��У�  
	void OnStop() override;

private:
	IService *m_interface = nullptr;
	bool m_bRun = false;
};
