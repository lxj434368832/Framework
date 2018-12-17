#include "../include/ConfigManage.h"

ConfigManage::ConfigManage(std::string strFileName)
{
	m_strConfigFile = strFileName;
}

ConfigManage::~ConfigManage()
{
}

void ConfigManage::SetValue(const std::string prefix, const std::string key, const std::string value)
{
	::WritePrivateProfileString(prefix.c_str(), key.c_str(), value.c_str(), m_strConfigFile.c_str());
}

void ConfigManage::SetDefault(const std::string prefix, const std::string key, const std::string value)
{
	char strValue[256];
	if (0 == GetPrivateProfileString(prefix.c_str(), key.c_str(), "", strValue, 256, m_strConfigFile.c_str()))
	{
		::WritePrivateProfileString(prefix.c_str(), key.c_str(), value.c_str(), m_strConfigFile.c_str());
	}
}

std::string ConfigManage::GetValue(const std::string prefix, const std::string key, const std::string defaultValue)
{
	char strValue[256];
	GetPrivateProfileString(prefix.c_str(), key.c_str(), defaultValue.c_str(), strValue, 256, m_strConfigFile.c_str());
	return strValue;
}

void ConfigManage::SetValueInt(const std::string prefix, const std::string key, const int value)
{
	::WritePrivateProfileString(prefix.c_str(), key.c_str(), std::to_string(value).c_str(), m_strConfigFile.c_str());
}

void ConfigManage::SetDefaultInt(const std::string prefix, const std::string key, const int value)
{
	char strValue[256];
	if (0 == GetPrivateProfileString(prefix.c_str(), key.c_str(), "", strValue, 256, m_strConfigFile.c_str()))
	{
		::WritePrivateProfileString(prefix.c_str(), key.c_str(), std::to_string(value).c_str(), m_strConfigFile.c_str());
	}
}

int ConfigManage::GetValueInt(const std::string prefix, const std::string key, const int defaultValue)
{
	return ::GetPrivateProfileInt(prefix.c_str(), key.c_str(), -1, m_strConfigFile.c_str());
}
