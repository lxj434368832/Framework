#pragma once
/************************************************************************
* function：服务功能具体的实现类
* author :	明巧文
* datetime：2018-04-03
* company:  安碧捷科技股份有限公司
/************************************************************************/

#include "../../include/ServiceManage/ServiceBase.h"

class IService;

//此类继承基础服务类
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
	//服务开启时调用的接口（此接口不能阻塞，如果阻塞服务会一直显示开启中）  
	void OnStart(DWORD argc, TCHAR* argv[]) override;

	//此步阻塞服务
	void OnContinueRun() override;

	//服务关闭时调用的接口（此接口不能阻塞，如果阻塞服务会一直显示关闭中）  
	void OnStop() override;

private:
	IService *m_interface = nullptr;
	bool m_bRun = false;
};
