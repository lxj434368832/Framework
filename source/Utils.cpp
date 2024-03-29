#include <winsock2.h>
#include <iphlpapi.h>
#include "../include/utils.h"
#include "../include/CommonDefine.h"
#include <time.h>

#pragma comment(lib,"IPHLPAPI.lib")


//*****************************************************************************
//	函数说明：	获得模块路径
//	函数原型：	void get_module_path(HMODULE module, char *f_name, char *out_f_n)
//  参数：      module模块句柄、f_name文件名、out_f_n包括路径在内的全名
//  返回值：    无
//  用法：		如果module为NULL，获取的文件名为(自身模块路径+f_name)
//*****************************************************************************
std::string mqwUtils::GetModulePath(HMODULE module, const char *f_name)
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
HMODULE mqwUtils::GetCurrentModuleHandle()
{
	MEMORY_BASIC_INFORMATION mbi;
	return ((VirtualQuery(GetCurrentModuleHandle, &mbi, sizeof(mbi)) != 0) ? (HMODULE) mbi.AllocationBase : NULL);
}

//*****************************************************************************
//	函数说明：	判断模块名
//	函数原型：	bool is_process_a(HMODULE Module, char *pname)
//  参数：      module需要判断的模块句柄、pname需要判断的模块名
//  返回值：    如果模块句柄与模块名正确返回true，否则返回false
//  用法：		
//*****************************************************************************
bool mqwUtils::IsModuleNameMatch(HMODULE module, char *pName)
{
	char cPath[MAX_PATH] = {0};
	char cFile[MAX_PATH] = {0};
	GetModuleFileNameA(module, cPath, MAX_PATH);
	_splitpath(cPath, 0, 0, cFile, 0);
	if (!stricmp(cFile, pName))
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
int mqwUtils::GetProcessorCount()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
}

std::string mqwUtils::MakeGuid()
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

std::string mqwUtils::GetErrorMessage(DWORD dw)
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

bool mqwUtils::GetMacByGetAdaptersInfo(std::string & macOUT)
{
	bool ret = false;
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO pAdapterInfo =
		(IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	if (nullptr == pAdapterInfo) return ret;
	// Make an initial call to GetAdaptersInfo to get the necessary size into the ulOutBufLen variable
	if (ERROR_BUFFER_OVERFLOW == GetAdaptersInfo(pAdapterInfo, &ulOutBufLen))
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
		if (nullptr == pAdapterInfo) return false;
	}

	if (NO_ERROR == GetAdaptersInfo(pAdapterInfo, &ulOutBufLen))
	{
		for (PIP_ADAPTER_INFO pAdapter = pAdapterInfo; nullptr != pAdapter; pAdapter = pAdapter->Next)
		{
			// 确保是以太网 
			if (MIB_IF_TYPE_ETHERNET != pAdapter->Type ) continue;
			char acMAC[32];
			sprintf(acMAC, "%02X-%02X-%02X-%02X-%02X-%02X",
				int(pAdapter->Address[0]),
				int(pAdapter->Address[1]),
				int(pAdapter->Address[2]),
				int(pAdapter->Address[3]),
				int(pAdapter->Address[4]),
				int(pAdapter->Address[5]));
			macOUT = acMAC;
			ret = true;
			break;
		}
	}
	free(pAdapterInfo);
	return ret;
}

bool mqwUtils::GetMacByGetAdaptersAddresses(std::string & macOUT)
{
	bool ret = false;
	ULONG outBufLen = sizeof(IP_ADAPTER_ADDRESSES);
	PIP_ADAPTER_ADDRESSES pAddresses =
		(IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
	if (nullptr == pAddresses) return ret;

	// Make an initial call to GetAdaptersAddresses to get the necessary size into the ulOutBufLen variable
	if (ERROR_BUFFER_OVERFLOW == GetAdaptersAddresses(AF_UNSPEC,0,nullptr, pAddresses, &outBufLen))
	{
		free(pAddresses);
		pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
		if (nullptr == pAddresses) return ret;
	}

	if (NO_ERROR == GetAdaptersAddresses(AF_UNSPEC, 0, nullptr, pAddresses, &outBufLen))
	{
		// If successful, output some information from the data we received
		for (PIP_ADAPTER_ADDRESSES pCurrAddress = pAddresses; nullptr != pCurrAddress; pCurrAddress = pCurrAddress->Next )
		{
			// 确保MAC地址的长度为 00-00-00-00-00-00 
			if(6 != pCurrAddress->PhysicalAddressLength) continue;
			char acMAC[32];
			sprintf_s(acMAC, "%02X-%02X-%02X-%02X-%02X-%02X",
				int(pCurrAddress->PhysicalAddress[0]),
				int(pCurrAddress->PhysicalAddress[1]),
				int(pCurrAddress->PhysicalAddress[2]),
				int(pCurrAddress->PhysicalAddress[3]),
				int(pCurrAddress->PhysicalAddress[4]),
				int(pCurrAddress->PhysicalAddress[5]));
			macOUT = acMAC;
			ret = true;
			break;
		}
	}

	free(pAddresses);
	return ret;
}


MElapsedTimer::MElapsedTimer()
{
	Restart();
}

void MElapsedTimer::Restart()
{
	_start = std::chrono::high_resolution_clock::now();
}

uint64_t MElapsedTimer::ElapsedMicroseconds()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> end
		= std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(end - _start).count();
}

uint64_t MElapsedTimer::Elapsednanoseconds()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> end
		= std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(end - _start).count();
}
