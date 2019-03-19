#pragma once

#include "HttpRequest.h"

class HttpUpload : public HttpRequest
{
public:
	//HttpUpload();
	//~HttpUpload();
	/*************************************************************************
	* function：投递一个请求
	* param strUrl:  in，请求的url地址
	* param strPost: in,请求的数据
	* param strResponse: out,返回的数据
	* return:		返回投递的结果命令,0为成功
	*************************************************************************/
	int PerformRequest(const std::string &strUrl, const std::string &strFileName, std::string& strResponse) override;

private:
	//请求数据回调的处理函数
	virtual size_t DataCallback(char *buffer, size_t size, size_t nitems) override;
};