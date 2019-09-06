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
	//����url��ַ
	virtual int SetRequestUrl(const std::string& strUrl) = 0;
	//��������ͷ
	virtual int SetRequestHeader(const std::map<std::string, std::string>& headers) = 0;

	/*************************************************************************
	* function��Ͷ��һ������
	* param strUrl:  in�������url��ַ
	* param strPost: in,���������
	* param strResponse: out,���ص�����
	* return:		����Ͷ�ݵĽ������,0Ϊ�ɹ�
	*************************************************************************/
	virtual int PerformRequest(const std::string &strUrl, const std::string &strData, std::string& strResponse)=0;

	/*************************************************************************
	* function��ֹͣ��ǰ����
	*************************************************************************/
	void StopRequest();

protected:
	bool			m_bStart;			//�Ƿ�ʼ��ʶ
	HANDLE			m_hCurl;			//curl����ľ��
	curl_slist		*m_pHeadList;		//curlͷ�б�

	std::string     m_strReceiveHead;	//���ص�ͷ������
	void*			m_lpData;			//�û�����ָ��
};
