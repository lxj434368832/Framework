#pragma once

#include "IHttpRequest.h"

class HttpRequest : public IHttpRequest
{
public:
	HttpRequest();
	~HttpRequest();

	//����url��ַ
	int SetRequestUrl(const std::string& strUrl) override;
	//��������ͷ
	int SetRequestHeader(const std::map<std::string, std::string>& headers) override;

	/*************************************************************************
	* function��Ͷ��һ������
	* param strUrl:  in�������url��ַ
	* param strPost: in,���������
	* param strResponse: out,���ص�����
	* return:		����Ͷ�ݵĽ������,0Ϊ�ɹ�
	*************************************************************************/
	int PerformRequest(const std::string &strUrl, const std::string &strData, std::string& strResponse) override;

protected:
	//ͷ�ص�
	static size_t HeaderCallback(char *buffer, size_t size, size_t nitems, void *userdata);
	//�������ݻص�
	static size_t DataCallbackStatic(char *buffer, size_t size, size_t nitems, void *userdata);

	//�������ݻص��Ĵ�����
	virtual size_t DataCallback(char *buffer, size_t size, size_t nitems);
};
