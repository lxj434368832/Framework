#include "../include/utils.h"
#include "../include/CommonDefine.h"
#include <time.h>

using namespace Utils;

//*****************************************************************************
//	函数说明：	获得模块路径
//	函数原型：	void get_module_path(HMODULE module, char *f_name, char *out_f_n)
//  参数：      module模块句柄、f_name文件名、out_f_n包括路径在内的全名
//  返回值：    无
//  用法：		如果module为NULL，获取的文件名为(自身模块路径+f_name)
//*****************************************************************************
std::string Utils::get_module_path(HMODULE module, const char *f_name)
{
	char cpath[MAX_PATH] = {0};
	char cdrive[MAX_PATH] = {0};
	char cdir[MAX_PATH] = {0};
    GetModuleFileNameA(module, cpath, MAX_PATH);
    _splitpath(cpath, cdrive, cdir, 0, 0);
	std::string strPath(cdrive);
	strPath.append(cdir).append(f_name);
    return strPath;
}

//*****************************************************************************
//	函数说明：	获得当前模块句柄包括dll模块
//	函数原型：	HMODULE get_self_module_handle()
//  参数：      无
//  返回值：    当前模块句柄
//  用法：		
//*****************************************************************************
HMODULE Utils::get_self_module_handle()
{
	MEMORY_BASIC_INFORMATION mbi;
	return ((VirtualQuery(get_self_module_handle, &mbi, sizeof(mbi)) != 0) ? (HMODULE) mbi.AllocationBase : NULL);
}

//*****************************************************************************
//	函数说明：	判断模块名
//	函数原型：	bool is_process_a(HMODULE Module, char *pname)
//  参数：      module需要判断的模块句柄、pname需要判断的模块名
//  返回值：    如果模块句柄与模块名正确返回true，否则返回false
//  用法：		
//*****************************************************************************
bool Utils::is_process_a(HMODULE module, char *pname)
{
	char cPath[MAX_PATH] = {0};
	char cFile[MAX_PATH] = {0};
	GetModuleFileNameA(module, cPath, MAX_PATH);
	_splitpath(cPath, 0, 0, cFile, 0);
	if (!stricmp(cFile, pname)) 
		return true;
	return false;
}

//*****************************************************************************
//	函数说明：	获得处理器的数量
//	函数原型：	int get_num_of_processors()
//  参数：      无
//  返回值：    数量
//  用法：		
//*****************************************************************************
int Utils::get_num_of_processors()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
}

std::string Utils::make_guid()
{
	char buf[64] = { 0 };
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		sprintf_s(buf, sizeof(buf)
			, "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]
			);
	}
	return std::string(buf);
}

std::string Utils::GetErrorMessage(DWORD dw)
{
	{
		LPTSTR lpMsgBuf = NULL;

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			lpMsgBuf,
			0, NULL);
		std::string strError(lpMsgBuf);
		LocalFree(lpMsgBuf);
		return strError;
	}
}

//*****************************************************************************
//	函数说明：	判断是否大于时间
//	函数原型：	bool is_time(ULONG &ms, ULONG dms)
//  参数：      ms前一个记录时间、dms时间间隔
//  返回值：    如果当前时间比之前记录时间大于时间间隔，返回true，否则返回false
//  用法：		用来判断是否超时
//*****************************************************************************
bool Utils::is_time(time_t ms, time_t dms)
{
    time_t sms = time(0);
	return (sms - ms) > dms;
}

//*****************************************************************************
//	函数说明：	判断是否大于时间，如果大于则修改
//	函数原型：	bool is_time_v(ULONG &ms, ULONG dms)
//  参数：      同上
//  返回值：    大于则修改返回true，否则返回false
//  用法：		判断是否超时，如果超时就修改记录时间为当前时间
//*****************************************************************************
bool Utils::is_time_v(time_t &ms, time_t dms)
{
    time_t sms = time(0);
	if((sms - ms) > dms)
    {
        ms = sms; 
        return true;
    }
	return false;
}
