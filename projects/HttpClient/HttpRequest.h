#pragma once

#include "IHttpRequest.h"

class HttpRequest : public IHttpRequest
{
public:
	HttpRequest();
	~HttpRequest();

	//设置url地址
	int SetRequestUrl(const std::string& strUrl) override;
	//设置请求头
	int SetRequestHeader(const std::map<std::string, std::string>& headers) override;

	/*************************************************************************
	* function：投递一个请求
	* param strUrl:  in，请求的url地址
	* param strPost: in,请求的数据
	* param strResponse: out,返回的数据
	* return:		返回投递的结果命令,0为成功
	*************************************************************************/
	int PerformRequest(const std::string &strUrl, const std::string &strData, std::string& strResponse) override;

protected:
	//头回调
	static size_t HeaderCallback(char *buffer, size_t size, size_t nitems, void *userdata);
	//请求数据回调
	static size_t DataCallbackStatic(char *buffer, size_t size, size_t nitems, void *userdata);

	//请求数据回调的处理函数
	virtual size_t DataCallback(char *buffer, size_t size, size_t nitems);
};
