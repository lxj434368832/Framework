#pragma once
/*******************************************************************
* @版权信息：
* @文件名称：LogFile.h
* @摘    要：日志操作定义头文件
* @作    者：明巧文
* @当前版本：1.0.0
* @日    期：2019年5月21日
* @备    注：线程安全日志类
*******************************************************************/

#include <sstream>
#include <list>

#define LOG_BUFFER_SIZE 512

class SingleLog
{
public:
	SingleLog();
	~SingleLog();
	void AddLog(const char* format, ...);

	template <typename T>
	inline SingleLog& operator<<(const T& log)
	{
		m_strStream << log;
		return *this;
	}

	inline SingleLog& operator<<(std::ostream& (*log)(std::ostream&))
	{
		m_strStream << log;
		return *this;
	}

private:
	char	m_szLineLog[LOG_BUFFER_SIZE];
	std::stringstream m_strStream;
};

#define Log() (SingleLog())<< "["<<__FUNCTION__<<":"<<__LINE__<< "]"
#define logm() Log()<<"<Info> "
#define logd() Log()<<"<Debug> "
#define logw() Log()<<"<Warn> "
#define loge() Log()<<"<Error> "

//以下调用方式的消息长度不能超过256
#define LOGM(format, ...)  do{  SingleLog log; \
    log.AddLog("[%s:%d]<Info> ", __FUNCTION__, __LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGD(format, ...)  do{  SingleLog log; \
    log.AddLog("[%s:%d]<Debug> ",__FUNCTION__,__LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGW(format, ...)  do{  SingleLog log; \
    log.AddLog("[%s:%d]<Warn> ",__FUNCTION__,__LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGE(format, ...)  do{  SingleLog log; \
    log.AddLog("[%s:%d]<Error> ",__FUNCTION__,__LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
