#include "../../include/HttpClient/httpHeader.h"
#include "../../include/HttpClient/HttpClient.h"
#include "../../include/curl/curl.h"
#include <iostream>
#define loge() std::cout
#define logm() std::cout

HttpClient::HttpClient() :m_pHeadList(0)
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
	m_hCurl = NULL;
	m_pHeadList = NULL;
}

HttpClient::~HttpClient()
{
	if (m_hCurl)
	{
		curl_easy_cleanup(m_hCurl);
	}
	if (m_pHeadList)
	{
		curl_slist_free_all(m_pHeadList);
	}

	curl_global_cleanup();
}

void HttpClient::SetAccessKey(LPCTSTR stdId, LPCTSTR strSecret)
{
	m_strKeyId = stdId;
	m_strKeySecret = strSecret;
}

int HttpClient::SetRequestUrl(const std::string& strUrl)
{
	if (NULL == m_hCurl)
	{
		m_hCurl = curl_easy_init();
	}

	if (m_hCurl)
	{
		if (strUrl.substr(0, 5) == "https")
		{
			curl_easy_setopt(m_hCurl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(m_hCurl, CURLOPT_SSL_VERIFYHOST, 0L);
		}

		return curl_easy_setopt(m_hCurl, CURLOPT_URL, strUrl.c_str());
	}
	return CURLE_FAILED_INIT;
}

int HttpClient::SetRequestHeader(const std::map<std::string, std::string>& headers)
{
	int iRet = CURLE_FAILED_INIT;

	if (NULL == m_hCurl)
	{
		m_hCurl = curl_easy_init();
	}
	if (m_hCurl)
	{
		for (auto it = headers.begin(); it != headers.end(); ++it)
		{
			std::string header(it->first);
			header.append(":").append(it->second);

			m_pHeadList = curl_slist_append(m_pHeadList, header.c_str());
			if (NULL == m_pHeadList) return iRet;
		}
		iRet = curl_easy_setopt(m_hCurl, CURLOPT_HTTPHEADER, m_pHeadList);
	}
	return iRet;
}

int HttpClient::Post(const std::string &strUrl, const std::string &strPost, std::string& strResponse)
{
	int iRet;
	iRet = SetRequestUrl(strUrl);
	if (iRet) return iRet;

	http_request_header httpHead(HTTP_METHOD::HTTP_METHOD_POST, strPost.length(), m_strKeyId, m_strKeySecret, m_strUserId);
	iRet = SetRequestHeader(httpHead.GetHttpHead());
	if (iRet) return iRet;

	curl_easy_setopt(m_hCurl, CURLOPT_POST, 1);
	curl_easy_setopt(m_hCurl, CURLOPT_POSTFIELDS, strPost.c_str());
	curl_easy_setopt(m_hCurl, CURLOPT_POSTFIELDSIZE, strPost.size());

	curl_easy_setopt(m_hCurl, CURLOPT_HEADERFUNCTION, HttpClient::HeaderCallback);
	curl_easy_setopt(m_hCurl, CURLOPT_HEADERDATA, &m_strReceiveHead);
	curl_easy_setopt(m_hCurl, CURLOPT_WRITEFUNCTION, HttpClient::DataCallback);
	curl_easy_setopt(m_hCurl, CURLOPT_WRITEDATA, &strResponse);
	curl_easy_setopt(m_hCurl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(m_hCurl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(m_hCurl, CURLOPT_CONNECTTIMEOUT_MS, 3000);
	curl_easy_setopt(m_hCurl, CURLOPT_TIMEOUT, 3);  //超时3秒

	iRet = curl_easy_perform(m_hCurl);

	if (iRet != CURLE_OK)
	{
		loge() << "curl_easy_perform failed:" << curl_easy_strerror((CURLcode)iRet);
	}
	else
	{
		curl_easy_getinfo(m_hCurl, CURLINFO_RESPONSE_CODE, &iRet);
		if (CURLE_OK != iRet && 200 != iRet && 500 != iRet)
		{
			loge() << "http post respond error:" << curl_easy_strerror((CURLcode)iRet);
		}
		else
		{
			logm() << "getInfo code is :" << iRet;
			iRet = CURLE_OK;
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

int HttpClient::DownloadFile(const std::string &strUrl, const std::string &strFileName)
{
	int iRet;
	iRet = SetRequestUrl(strUrl);
	if (iRet) return iRet;

	http_request_header httpHead(HTTP_METHOD::HTTP_METHOD_POST, 0, m_strKeyId, m_strKeySecret, m_strUserId);
	iRet = SetRequestHeader(httpHead.GetHttpHead());
	if (iRet) return iRet;

	FILE *file;
	if (fopen_s(&file, strFileName.c_str(), "wb"))
	{
		loge() << "打开文件失败！文件路径：" << strFileName.c_str();
		return CURLE_FAILED_INIT;
	}

	curl_easy_setopt(m_hCurl, CURLOPT_WRITEFUNCTION, HttpClient::DownloadCallback);
	curl_easy_setopt(m_hCurl, CURLOPT_WRITEDATA, file);
	curl_easy_setopt(m_hCurl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(m_hCurl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(m_hCurl, CURLOPT_CONNECTTIMEOUT_MS, 1000);
	//curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);

	iRet = curl_easy_perform(m_hCurl);
	fclose(file);

	if (iRet != CURLE_OK)
	{
		loge() << "curl_easy_perform failed:" << curl_easy_strerror((CURLcode)iRet);
	}
	else
	{
		curl_easy_getinfo(m_hCurl, CURLINFO_RESPONSE_CODE, &iRet);
		if (CURLE_OK != iRet && 200 != iRet && 500 != iRet)
		{
			loge() << "http post respond error:" << curl_easy_strerror((CURLcode)iRet);
		}
		else
		{
			logm() << "getInfo code is :" << iRet;
			iRet = CURLE_OK;
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

std::string HttpClient::MakeUrl(const char* remote_address, const char* cmd_name)
{
	return std::string(remote_address).append(cmd_name);
}

size_t HttpClient::HeaderCallback(char *buffer, size_t size, size_t nitems, void *userdata)
{
	std::string* receive_header = reinterpret_cast<std::string*>(userdata);
	if (receive_header && buffer)
	{
		receive_header->append(buffer, size * nitems);
	}

	return nitems * size;
}

size_t HttpClient::DataCallback(char *buffer, size_t size, size_t nitems, void *userdata)
{
	std::string* data = reinterpret_cast<std::string*>(userdata);
	if (data && buffer)
	{
		data->append(buffer, size * nitems);
	}

	return nitems * size;
}

size_t HttpClient::DownloadCallback(char *buffer, size_t size, size_t nitems, void *lpVoid)
{
	if (NULL == lpVoid || NULL == buffer)
	{
		loge()<<"DownloadPage error!";
		return -1;
	}

	size_t count = fwrite(buffer, size, nitems, (FILE*)lpVoid);
	return size * count;
	return 0;
}
