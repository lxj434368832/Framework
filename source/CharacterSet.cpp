#include "../include/CharacterSet.h"
#include "../include/CommonDefine.h"
#include <iostream>


std::wstring mqwUtils::ANSIToUnicode(const char* strANSI)
{
	std::wstring strUnicode;
	int iSize = MultiByteToWideChar(CP_ACP, 0, strANSI, -1, NULL, 0);
	strUnicode.resize(iSize - 1);
	MultiByteToWideChar(CP_ACP, 0, strANSI, -1, (LPWSTR)strUnicode.data(), iSize);
	return strUnicode;
}

std::string mqwUtils::UnicodeToANSI(const wchar_t* strUnicode)
{
	std::string strANSI;
	int iSize = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	strANSI.resize(iSize - 1);
	WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, (LPSTR)strANSI.data(), iSize, NULL, NULL);
	return strANSI;
}

std::wstring mqwUtils::UTF8ToUnicode(const char* strUTF8)
{
	std::wstring strUnicode;
	int iSize = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
	strUnicode.resize(iSize - 1);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, (LPWSTR)strUnicode.data(), iSize);
	return strUnicode;
}

std::string mqwUtils::UnicodeToUTF8(const wchar_t* strUnicode)
{
	std::string strUTF8;
	int iSize = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	strUTF8.resize(iSize - 1);
	WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, (LPSTR)strUTF8.data(), iSize, NULL, NULL);
	return strUTF8;
}

std::string mqwUtils::ANSIToUTF8(const char* strANSI)
{
	std::wstring strUnicode = ANSIToUnicode(strANSI);
	return  UnicodeToUTF8(strUnicode.data());
}

std::string mqwUtils::UTF8ToANSI(const char* strUTF8)
{
	std::wstring strUnicode = UTF8ToUnicode(strUTF8);
	return UnicodeToANSI(strUnicode.data());
}

void mqwUtils::replace(LPSTR source, char look, char dest)
{
	int iLen = ::strlen(source);
	for (int i = 0; i < iLen; i++)
	{
		if (source[i] == look)
		{
			source[i] = dest;
		}
	}
}

//*****************************************************************************
//	����˵����	�ڴ��е��ַ���ת��Ϊ����ı���ʽ�����ڴ��ַ�������3aA-> 33 61 41
//	����ԭ�ͣ�	ULONG bin_to_hex(char *src, ULONG len, char * out, bool mode, char *ktext, int hlen)
//  ������      hlenÿ����ʾ������ktext�ָ���
//  ����ֵ��    
//  �÷���		
//*****************************************************************************
ULONG mqwUtils::bin_to_hex(const char *src, ULONG len, char * out, bool mode, char *ktext, int hlen)
{
	char *hout = out;
	char *ctext = "0123456789ABCDEF";
	if (mode)
		ctext = "0123456789abcdef";
	for (ULONG i = 0; i < len; i++)
	{
		*out = ctext[(BYTE)(src[i]) >> 4];
		out++;
		*out = ctext[src[i] & 0x0F];
		out++;
		if ((i % hlen) == (hlen - 1))
		{
			*(WORD *)(out) = 0x0A0D;
			out += 2;
		}
		else if (ktext && *ktext)
		{
			*out = *ktext;
			out++;
		}
	}
	*out = '\0';
	return out - hout;
}

//*****************************************************************************
//	����˵����	�ڴ������ı���ʽת��Ϊ�ڴ��ַ������ݣ����ڴ����ݵ��ı���ʽ33 61 41 -> 3aA
//	����ԭ�ͣ�	ULONG hex_to_bin(char *src, ULONG len, char *out)
//  ������      
//  ����ֵ��    
//  �÷���		
//*****************************************************************************
ULONG mqwUtils::hex_to_bin(const char *src, ULONG len, char *out)
{
	BYTE num, bit, bits = 0;
	ULONG i = 0, ti = 0;
	while (true)
	{
		num = src[ti];
		ti++;
		if (num >= 0x30 && num <= 0x39)
			num -= 0x30;
		else if (num >= 0x41 && num <= 0x46)
			num -= 0x37;
		else if (num >= 0x61 && num <= 0x66)
			num -= 0x57;
		else if ((!len && !num) || (len && ti >= len))
			break;
		else
			continue;

		if (bits)
		{
			bit |= num;
			out[i] = bit;
			i++;
		}
		else
			bit = num << 4;
		bits ^= 1;
	}
	return i;
}

//*****************************************************************************
//	����˵����	64bit��λ��λ
//	����ԭ�ͣ�	DWORD64 bswap_64(DWORD64 value)
//  ������      value��Ҫ��λ��ֵ
//  ����ֵ��    ���ػ�λ֮���ֵ
//  �÷���		
//*****************************************************************************
uint64_t mqwUtils::bswap_64(uint64_t value)
{
	return (((uint64_t)bswap_32(value)) << 32) | bswap_32((value >> 32));;
}

//*****************************************************************************
//	����˵����	32bit��λ
//	����ԭ�ͣ�	ͬ��
//  ������      ͬ��
//  ����ֵ��    ͬ��
//  �÷���		ͬ��
//*****************************************************************************
ULONG mqwUtils::bswap_32(ULONG value)
{
	_asm mov eax, value
	_asm bswap eax
}

//*****************************************************************************
//	����˵����	16bit��λ
//	����ԭ�ͣ�	ͬ��
//  ������      ͬ��
//  ����ֵ��    ͬ��
//  �÷���		ͬ��
//*****************************************************************************
USHORT mqwUtils::bswap_16(ULONG value)
{
	_asm mov eax, value
	_asm and eax, 0xFFFF
	_asm xchg ah, al
}

//setlocale(LC_CTYPE, "");
wchar_t* mqwUtils::Acsi2WideByte(const char* mbs)
{
	int len;
	wchar_t* buf;
	len = mbstowcs(nullptr, mbs, 0);
	if (len == 0)
		return NULL;
	buf = (wchar_t *)malloc(sizeof(wchar_t)*(len + 1));
	memset(buf, 0, sizeof(wchar_t) *(len + 1));
	len = mbstowcs(buf, mbs, len + 1);
	return buf;
}

char* mqwUtils::w2m(const wchar_t* wcs)
{
	int len;
	char* buf;
	len = wcstombs(NULL, wcs, 0);
	if (len == 0)
		return NULL;
	buf = (char *)malloc(sizeof(char)*(len + 1));
	memset(buf, 0, sizeof(char) *(len + 1));
	len = wcstombs(buf, wcs, len + 1);
	return buf;
}


#include <codecvt>
#include<memory>

/*wstring_convert�ṩ���ַ��ַ��������ֽ��ַ�����ת��,��codecvt_utf8�����ṩUTF-8�ı������
�����ӾͿ���ʵ��UTF8��Unicode�Ļ���ת����*/
std::string Unicode2UTF8(const std::wstring & wstr)
{
	std::string ret;
	try {
		std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
		ret = wcv.to_bytes(wstr);
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
	return ret;
}

std::wstring mqwUtils::UTF82Unicode(const std::string & str)
{
	std::wstring ret;
	try {
		std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
		ret = wcv.from_bytes(str);
	}
	catch (const std::exception & e) 
	{
		std::cerr << e.what() << std::endl;
	}
	return ret;
}

std::string mqwUtils::Unicode2ANSI(const std::wstring & wstr)
{
	std::string ret;
	std::mbstate_t state = {};
	const wchar_t *src = wstr.data();
	size_t len = std::wcsrtombs(nullptr, &src, 0, &state);
	if (-1 != len) 
	{
		std::unique_ptr< char[] > buff(new char[len + 1]);
		len = std::wcsrtombs(buff.get(), &src, len, &state);

		if (-1 != len) 
		{
			ret.assign(buff.get(), len);
		}
	}
	return ret;
}

std::wstring mqwUtils::ANSI2Unicode(const std::string & str)
{
	std::wstring ret;
	std::mbstate_t state = {};
	const char *src = str.data();
	size_t len = std::mbsrtowcs(nullptr, &src, 0, &state);
	if (-1 != len) 
	{
		std::unique_ptr< wchar_t[] > buff(new wchar_t[len + 1]);
		len = std::mbsrtowcs(buff.get(), &src, len, &state);
		if (-1 != len) 
		{
			ret.assign(buff.get(), len);
		}
	}
	return ret;
}

std::string UTF8ToANSI(const std::string & str)
{
	//return UnicodeToANSI(UTF8ToUnicode(str));
	return std::string();
}

std::string ANSIToUTF8(const std::string & str)
{
	//return UnicodeToUTF8(ANSIToUnicode(str));
	return std::string();
}
