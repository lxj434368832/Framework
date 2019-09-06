#pragma once
/************************************************************************
* function：HTTP通讯的头文件
* author :	明巧文
* datetime：2018-04-03
* company:  安碧捷科技股份有限公司
/************************************************************************/

#include <windows.h>
#include <string>
#include <map>
#include <list>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>

/*************************************************************************
* cmd_name,req_moudle_id,respond data,respond result
*************************************************************************/
typedef std::function<void(const std::string&, int, const std::string&, int)> fnReceiveCallback;

struct SClientContext 
{
	std::string strUrl;
	std::string strCmdName;
	int			iModuleId = 0;
	std::string strRequestData;
};

class IHttpRequest;

class HttpClient
{
public:
	HttpClient();
	~HttpClient();

	/*************************************************************************
	* function：	注册或注销组件接收数据的回调函数
	* param strCmd:  in，秘钥ID
	* param fn: in,秘钥密码
	*************************************************************************/
	void AdviseCallback(const std::string& strCmd, fnReceiveCallback fn);

	/*************************************************************************
	* function：	设置授权信息
	* param stdId:  in，秘钥ID
	* param strSecret: in,秘钥密码
	*************************************************************************/
	void SetAccessKey(LPCTSTR stdId, LPCTSTR strSecret);

	/*************************************************************************
	* function：投递一个请求
	* param strUrl:  in，请求的url地址
	* param strCmdName:  in，请求的命令名
	* param strPost: in,请求的数据
	* param strResponse: out,返回的数据
	* return:		返回投递的结果命令,0为成功
	*************************************************************************/
	std::string SendRequest(const std::string strUrl, const std::string strCmdName, const std::string strData);

	/*************************************************************************
	* function：投递一个请求
	* param strUrl:  in，请求的url地址
	* param strCmdName:  in，请求的命令名
	* param strPost: in,请求的数据
	* param strResponse: out,返回的数据
	*************************************************************************/
	void SendRequestAsyn(const std::string strUrl, const std::string strCmdName, const std::string strData, int iModuleId = 0);

	/*************************************************************************
	* function：下载文件
	* param strUrl:  in，请求的url地址
	* param strFileName: in,下载的文件名
	* return:		返回投递的结果命令
	*************************************************************************/
	int DownloadFile(const std::string &strUrl, const std::string &strFileName);

	/*************************************************************************
	* function：异步下载文件
	* param strUrl:  in，请求的url地址
	* param strCmdName:  in，请求的命令名，用于查找下载的回调函数
	* param strFileName: in,下载的文件名
	* return:		返回投递的结果命令
	*************************************************************************/
	void DownloadFileAsyn(const std::string &strUrl, const std::string strCmdName, const std::string &strFileName, int iModuleId = 0);

	/*************************************************************************
	* function：同步上传文件
	* param strUrl:  in，请求的url地址
	* param strFileName: in,下载的文件名
	* return:		返回投递的结果命令
	*************************************************************************/
	int UploadFile(const std::string &strUrl, const std::string &strFileName);

	/*************************************************************************
	* function：异步上传文件
	* param strUrl:  in，请求的url地址
	* param strCmdName:  in，请求的命令名，用于查找下载的回调函数
	* param strFileName: in,下载的文件名
	* return:		返回投递的结果命令
	*************************************************************************/
	void UploadFileAsyn(const std::string &strUrl, const std::string strCmdName, const std::string &strFileName, int iModuleId = 0);

private:
	void RequestThreadProc();
	void UploadThreadProc();
	void DownloadThreadProc();

private:
	bool				m_bStart;
	std::thread				*m_thdRequest;
	std::list<SClientContext*>	m_listRequest;
	std::mutex			m_mtRequest;
	std::condition_variable	m_cvRequest;

	std::thread				*m_thdDownload;
	std::list<SClientContext*>	m_listDownload;
	std::mutex				m_mtDownload;
	std::condition_variable	m_cvDownload;
	IHttpRequest			*m_pCurrentSynDownload;	//此为支持直接停止当前的下载，具体根据实际业务扩展
	IHttpRequest			*m_pCurrentASynDownload;

	std::thread				*m_thdUpload;
	std::list<SClientContext*>	m_listUpload;
	std::mutex			m_mtUpload;
	std::condition_variable	m_cvUpload;
	IHttpRequest			*m_pCurrentSynUpload;	//此为支持直接停止当前的上传，具体根据实际业务扩展
	IHttpRequest			*m_pCurrentASynUpload;

	std::map<std::string, fnReceiveCallback> m_mapCallback;

	std::string		m_strKeyId;			 //秘钥ID
	std::string		m_strKeySecret;		 //秘钥密文
	std::string		m_strUserId;		//用户ID
};

