#include "HttpClient.h"
#include "../../include/curl/curl.h"
#include "../../include/GlobalDefine.h" 
#include "httpHeader.h"	 
#include "HttpRequest.h"
#include "HttpDownload.h"
#include "HttpUpload.h"


HttpClient::HttpClient()
{
	curl_global_init(CURL_GLOBAL_DEFAULT);

	m_thdRequest = nullptr;
	m_thdDownload = nullptr;
	m_pCurrentSynDownload = nullptr;
	m_pCurrentASynDownload = nullptr;
	m_thdUpload = nullptr;
	m_pCurrentSynUpload = nullptr;
	m_pCurrentASynUpload = nullptr;
	m_bStart = true;
}

HttpClient::~HttpClient()
{
	m_bStart = false;

	if (m_thdRequest)
	{
		if(m_thdRequest->joinable()) m_thdRequest->join();
		delete m_thdRequest;
		m_thdRequest = nullptr;
	}

	if (m_thdDownload)
	{
		if (m_thdDownload->joinable()) m_thdDownload->join();
		delete m_thdDownload;
		m_thdDownload = nullptr;
	}

	if (m_thdUpload)
	{
		if (m_thdUpload->joinable()) m_thdUpload->join();
		delete m_thdUpload;
		m_thdUpload = nullptr;
	}

	curl_global_cleanup();
}

void HttpClient::AdviseCallback(const std::string& strCmd, fnReceiveCallback fn)
{
	if (strCmd.empty())	return;
	if (fn)
	{
		m_mapCallback[strCmd] = fn;
	}
	else
		m_mapCallback.erase(strCmd);
}

void HttpClient::SetAccessKey(LPCTSTR stdId, LPCTSTR strSecret)
{
	m_strKeyId = stdId;
	m_strKeySecret = strSecret;
}

std::string HttpClient::SendRequest(const std::string strUrl, const std::string strCmdName, const std::string strData, int iModuleId)
{
	std::string strRespond;
	string strFullUrl = strUrl + strCmdName;

	http_request_header httpHead(HTTP_METHOD::HTTP_METHOD_POST, strData.length(), m_strKeyId, m_strKeySecret, m_strUserId);

	HttpRequest request;
	if (request.SetRequestHeader(httpHead.GetHttpHead()))
	{
		loge() << "��������ͷʧ�ܡ�";
		return strRespond;
	}

	if (request.PerformRequest(strFullUrl, strData, strRespond))
		loge() << "http����ʧ�ܣ�����urlΪ:" << strFullUrl;

	return strRespond;
}

void HttpClient::SendRequestAsyn(const std::string strUrl, const std::string strCmdName, const std::string strData, int iModuleId)
{
	SClientContext	*pContext = new SClientContext;
	pContext->strUrl = strUrl + strCmdName;
	pContext->strCmdName = strCmdName;
	pContext->iModuleId = iModuleId;
	pContext->strRequestData = strData;

	std::unique_lock<std::mutex> lck(m_mtRequest);
	if (nullptr == m_thdRequest)
	{
		m_thdRequest = new std::thread(&HttpClient::RequestThreadProc, this);
	}
	m_listRequest.push_back(pContext);
	m_cvRequest.notify_one();
}

void HttpClient::RequestThreadProc()
{
	int iRet = 0;

	while (m_bStart)
	{
		std::unique_lock<std::mutex> lck(m_mtRequest);
		if (0 == m_listRequest.size())
		{
			m_cvRequest.wait(lck);
		}

		if (m_listRequest.size())
		{
			SClientContext *pContext = m_listRequest.front();
			HttpRequest request;
			http_request_header httpHead(HTTP_METHOD::HTTP_METHOD_POST, pContext->strRequestData.length(), m_strKeyId, m_strKeySecret, m_strUserId);

			if (request.SetRequestHeader(httpHead.GetHttpHead()))
			{
				loge() << "��������ͷʧ�ܡ�";
				return;
			}

			std::string strRespondData;
			iRet = request.PerformRequest(pContext->strUrl, pContext->strRequestData, strRespondData);
			
			if (m_mapCallback.end() != m_mapCallback.find(pContext->strCmdName))
			{
				m_mapCallback[pContext->strCmdName](pContext->strCmdName, pContext->iModuleId, strRespondData, iRet);
			}
			else
			{
				loge() << "�����ڵ�ǰ����:"<< pContext->strCmdName<<" ��Ӧ�Ĵ�������";
			}
			delete pContext;
		}

	}
}

int HttpClient::DownloadFile(const std::string &strUrl, const std::string &strFileName)
{
	int iRet = 0;
	std::string strRespond;

	http_request_header httpHead(HTTP_METHOD::HTTP_METHOD_POST, 0, m_strKeyId, m_strKeySecret, m_strUserId);

	HttpDownload request;
	iRet = request.SetRequestHeader(httpHead.GetHttpHead());
	if (iRet) return iRet;

	m_pCurrentSynDownload = &request;
	if (iRet = request.PerformRequest(strUrl, strFileName, strRespond))
		loge() << "http�����ļ�ʧ�ܣ�����urlΪ:" << strUrl;

	m_pCurrentSynDownload = nullptr;
	return iRet;
}

void HttpClient::DownloadFileAsyn(const std::string &strUrl, const std::string strCmdName, const std::string &strFileName)
{
	SClientContext	*pContext = new SClientContext;
	pContext->strUrl = strUrl;
	pContext->strCmdName = strCmdName;
	//pContext->iModuleId = iModuleId;
	pContext->strRequestData = strFileName;

	std::unique_lock<std::mutex> lck(m_mtDownload);
	if (nullptr == m_thdDownload)
	{
		m_thdDownload = new std::thread(&HttpClient::DownloadThreadProc, this);
	}
	m_listDownload.push_back(pContext);
	m_cvDownload.notify_one();
}

void HttpClient::DownloadThreadProc()
{
	int iRet = 0;

	while (m_bStart)
	{
		std::unique_lock<std::mutex> lck(m_mtDownload);
		if (0 == m_listDownload.size())
		{
			m_cvDownload.wait(lck);
		}

		if (m_listDownload.size())
		{
			SClientContext *pContext = m_listDownload.front();
			HttpDownload request;
			http_request_header httpHead(HTTP_METHOD::HTTP_METHOD_POST, 0, m_strKeyId, m_strKeySecret, m_strUserId);

			if (request.SetRequestHeader(httpHead.GetHttpHead()))
			{
				loge() << "��������ͷʧ�ܡ�";
				return;
			}

			m_pCurrentASynDownload = &request;
			std::string strRespondData;
			iRet = request.PerformRequest(pContext->strUrl, pContext->strRequestData, strRespondData);

			if (m_mapCallback.end() != m_mapCallback.find(pContext->strCmdName))
			{
				m_mapCallback[pContext->strCmdName](pContext->strCmdName, pContext->iModuleId, strRespondData, iRet);
			}
			else
			{
				loge() << "�����ļ�ʧ�ܣ��ļ�urlΪ:" << pContext->strCmdName;
			}
			delete pContext;
			m_pCurrentASynDownload = nullptr;
		}

	}
}

int HttpClient::UploadFile(const std::string &strUrl, const std::string &strFileName)
{
	int iRet = 0;
	std::string strRespond;

	http_request_header httpHead(HTTP_METHOD::HTTP_METHOD_POST, 0, m_strKeyId, m_strKeySecret, m_strUserId);

	HttpUpload request;
	iRet = request.SetRequestHeader(httpHead.GetHttpHead());
	if (iRet) return iRet;

	m_pCurrentSynUpload = &request;
	if (iRet = request.PerformRequest(strUrl, strFileName, strRespond))
		loge() << "http�ϴ��ļ�ʧ�ܣ�����urlΪ:" << strUrl;

	m_pCurrentSynUpload = nullptr;
	return iRet;
}

void HttpClient::UploadFileAsyn(const std::string &strUrl, const std::string strCmdName, const std::string &strFileName)
{
	SClientContext	*pContext = new SClientContext;
	pContext->strUrl = strUrl;
	pContext->strCmdName = strCmdName;
	//pContext->iModuleId = iModuleId;
	pContext->strRequestData = strFileName;

	std::unique_lock<std::mutex> lck(m_mtUpload);
	if (nullptr == m_thdUpload)
	{
		m_thdUpload = new std::thread(&HttpClient::UploadThreadProc, this);
	}
	m_listUpload.push_back(pContext);
	m_cvUpload.notify_one();
}

void HttpClient::UploadThreadProc()
{
	int iRet = 0;

	while (m_bStart)
	{
		std::unique_lock<std::mutex> lck(m_mtUpload);
		if (0 == m_listUpload.size())
		{
			m_cvUpload.wait(lck);
		}

		if (m_listUpload.size())
		{
			SClientContext *pContext = m_listUpload.front();
			HttpUpload request;
			http_request_header httpHead(HTTP_METHOD::HTTP_METHOD_POST, 0, m_strKeyId, m_strKeySecret, m_strUserId);

			if (request.SetRequestHeader(httpHead.GetHttpHead()))
			{
				loge() << "��������ͷʧ�ܡ�";
				return;
			}

			m_pCurrentASynUpload = &request;
			std::string strRespondData;
			iRet = request.PerformRequest(pContext->strUrl, pContext->strRequestData, strRespondData);

			if (m_mapCallback.end() != m_mapCallback.find(pContext->strCmdName))
			{
				m_mapCallback[pContext->strCmdName](pContext->strCmdName, pContext->iModuleId, strRespondData, iRet);
			}
			else
			{
				loge() << "�ϴ��ļ�ʧ�ܣ��ļ�urlΪ:" << pContext->strCmdName;
			}
			delete pContext;
			m_pCurrentASynUpload = nullptr;
		}

	}
}
