#include "HttpUpload.h"		  					
#include "../../include/curl/curl.h"
#include "../../include/GlobalDefine.h" 
#include <io.h>

int HttpUpload::PerformRequest(const std::string &strUrl, const std::string &strFileName, std::string& strResponse)
{
	int iRet = 0;
	iRet = SetRequestUrl(strUrl);
	if (iRet) return iRet;

	FILE *file;
	if (fopen_s(&file, strFileName.c_str(), "wb"))
	{
		loge() << "���ϴ��ļ�ʧ�ܣ��ļ�·����" << strFileName.c_str();
		return CURLE_FAILED_INIT;
	}	 
	m_lpData = file;
	ULONG64 iFileSize = _filelength(_fileno(file));
	curl_easy_setopt(m_hCurl, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(m_hCurl, CURLOPT_READFUNCTION, HttpRequest::DataCallbackStatic);
	curl_easy_setopt(m_hCurl, CURLOPT_READDATA, this);
	curl_easy_setopt(m_hCurl, CURLOPT_INFILESIZE_LARGE, iFileSize);
	curl_easy_setopt(m_hCurl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(m_hCurl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(m_hCurl, CURLOPT_CONNECTTIMEOUT_MS, 1000);
	//curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);

	iRet = curl_easy_perform(m_hCurl);
	fclose(file);

	if (iRet != CURLE_OK)
	{
		loge() << "curl_easy_perform failed,error code:" << iRet << "msg:" << curl_easy_strerror((CURLcode)iRet);
	}
	else
	{
		curl_easy_getinfo(m_hCurl, CURLINFO_RESPONSE_CODE, &iRet);
		if (CURLE_OK == iRet || 200 == iRet || 500 == iRet)
		{
			logm() << "getInfo code is :" << iRet;
			iRet = CURLE_OK;
		}
		else
		{
			loge() << "http post respond error code:" << iRet << " msg:" << curl_easy_strerror((CURLcode)iRet);
		}
	}

	if (m_pHeadList)
	{
		curl_slist_free_all(m_pHeadList);
		m_pHeadList = NULL;
	}

	if (m_hCurl)
	{
		curl_easy_cleanup(m_hCurl);
		m_hCurl = NULL;
	}
	return iRet;
}

size_t HttpUpload::DataCallback(char *buffer, size_t size, size_t nitems)
{
	if (nullptr == m_lpData || nullptr == buffer)
	{
		loge() << "�ϴ��ļ���������!";
		return 0;
	}

	if (m_bStart)
		return fread(buffer, size, nitems, (FILE*)m_lpData);
	else
		return 0;
}
