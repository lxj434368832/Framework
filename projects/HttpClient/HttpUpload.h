#pragma once

#include "HttpRequest.h"

class HttpUpload : public HttpRequest
{
public:
	//HttpUpload();
	//~HttpUpload();
	/*************************************************************************
	* function��Ͷ��һ������
	* param strUrl:  in�������url��ַ
	* param strPost: in,���������
	* param strResponse: out,���ص�����
	* return:		����Ͷ�ݵĽ������,0Ϊ�ɹ�
	*************************************************************************/
	int PerformRequest(const std::string &strUrl, const std::string &strFileName, std::string& strResponse) override;

private:
	//�������ݻص��Ĵ�����
	virtual size_t DataCallback(char *buffer, size_t size, size_t nitems) override;
};