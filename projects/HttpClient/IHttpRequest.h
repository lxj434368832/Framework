#pragma once

#include <windows.h>
#include <string>
#include <map>

struct curl_slist;

class IHttpRequest
{
public:
	IHttpRequest();
	virtual ~IHttpRequest();
	//设置url地址
	virtual int SetRequestUrl(const std::string& strUrl) = 0;
	//设置请求头
	virtual int SetRequestHeader(const std::map<std::string, std::string>& headers) = 0;

	/*************************************************************************
	* function：投递一个请求
	* param strUrl:  in，请求的url地址
	* param strPost: in,请求的数据
	* param strResponse: out,返回的数据
	* return:		返回投递的结果命令,0为成功
	*************************************************************************/
	virtual int PerformRequest(const std::string &strUrl, const std::string &strData, std::string& strResponse)=0;

	/*************************************************************************
	* function：停止当前请求
	*************************************************************************/
	void StopRequest();

protected:
	bool			m_bStart;			//是否开始标识
	HANDLE			m_hCurl;			//curl处理的句柄
	curl_slist		*m_pHeadList;		//curl头列表

	std::string     m_strReceiveHead;	//返回的头部数据
	void*			m_lpData;			//用户数据指针
};
