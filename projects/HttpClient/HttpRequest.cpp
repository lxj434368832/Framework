#include "HttpRequest.h"		  				
#include "../../include/curl/curl.h"
#include "../../include/GlobalDefine.h" 


IHttpRequest::IHttpRequest()
{
	m_bStart = true;
	m_hCurl = nullptr;
	m_pHeadList = nullptr;
	m_lpData = nullptr;
}

IHttpRequest::~IHttpRequest()
{
	if (m_hCurl)
	{
		curl_easy_cleanup(m_hCurl);
	}
	if (m_pHeadList)
	{
		curl_slist_free_all(m_pHeadList);
	}
}

void IHttpRequest::StopCurrentRequest()
{
	m_bStart = false;
}



HttpRequest::HttpRequest():
	IHttpRequest()
{
}

HttpRequest::~HttpRequest()
{
}

int HttpRequest::SetRequestUrl(const std::string& strUrl)
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

int HttpRequest::SetRequestHeader(const std::map<std::string, std::string>& headers)
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

int HttpRequest::PerformRequest(const std::string &strUrl, const std::string &strData, std::string& strResponse)
{
	int iRet;
	iRet = SetRequestUrl(strUrl);
	if (iRet) return iRet;

	m_lpData = &strResponse;

	curl_easy_setopt(m_hCurl, CURLOPT_POST, 1);
	//curl_easy_setopt(m_hCurl, CURLOPT_HTTPPOST, 1);
	curl_easy_setopt(m_hCurl, CURLOPT_POSTFIELDS, strData.c_str());
	curl_easy_setopt(m_hCurl, CURLOPT_POSTFIELDSIZE, strData.size());

	curl_easy_setopt(m_hCurl, CURLOPT_HEADERFUNCTION, HttpRequest::HeaderCallback);
	curl_easy_setopt(m_hCurl, CURLOPT_HEADERDATA, &m_strReceiveHead);
	curl_easy_setopt(m_hCurl, CURLOPT_WRITEFUNCTION, HttpRequest::DataCallbackStatic);
	curl_easy_setopt(m_hCurl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(m_hCurl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(m_hCurl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(m_hCurl, CURLOPT_CONNECTTIMEOUT_MS, 3000);
	curl_easy_setopt(m_hCurl, CURLOPT_TIMEOUT, 3);  //≥¨ ±3√Î

	iRet = curl_easy_perform(m_hCurl);

	if (iRet != CURLE_OK)
	{
		loge() << "curl_easy_perform failed:" << curl_easy_strerror((CURLcode)iRet);
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

size_t HttpRequest::HeaderCallback(char *buffer, size_t size, size_t nitems, void *userdata)
{
	std::string* data = reinterpret_cast<std::string*>(userdata);
	if (data && buffer)
	{
		data->append(buffer, size * nitems);
	}

	return nitems * size;
}

size_t HttpRequest::DataCallbackStatic(char *buffer, size_t size, size_t nitems, void *userdata)
{
	HttpRequest* pThis = reinterpret_cast<HttpRequest*>(userdata);
	if (pThis)
	{
		return pThis->DataCallback(buffer, size, nitems);
	}
	else
		return 0;
}

size_t HttpRequest::DataCallback(char *buffer, size_t size, size_t nitems)
{
	std::string* data = reinterpret_cast<std::string*>(m_lpData);
	if (data && buffer)
	{
		data->append(buffer, size * nitems);
	}

	return nitems * size;
}

