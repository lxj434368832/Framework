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
//	函数说明：	内存中的字符串转换为输出文本格式，如内存字符串数据3aA-> 33 61 41
//	函数原型：	ULONG bin_to_hex(char *src, ULONG len, char * out, bool mode, char *ktext, int hlen)
//  参数：      hlen每行显示个数，ktext分隔符
//  返回值：    
//  用法：		
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
//	函数说明：	内存数据文本格式转换为内存字符串数据，如内存数据的文本格式33 61 41 -> 3aA
//	函数原型：	ULONG hex_to_bin(char *src, ULONG len, char *out)
//  参数：      
//  返回值：    
//  用法：		
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
//	函数说明：	64bit的位换位
//	函数原型：	DWORD64 bswap_64(DWORD64 value)
//  参数：      value需要换位的值
//  返回值：    返回换位之后的值
//  用法：		
//*****************************************************************************
uint64_t mqwUtils::bswap_64(uint64_t value)
{
	return (((uint64_t)bswap_32(value)) << 32) | bswap_32((value >> 32));;
}

//*****************************************************************************
//	函数说明：	32bit换位
//	函数原型：	同上
//  参数：      同上
//  返回值：    同上
//  用法：		同上
//*****************************************************************************
ULONG mqwUtils::bswap_32(ULONG value)
{
	_asm mov eax, value
	_asm bswap eax
}

//*****************************************************************************
//	函数说明：	16bit换位
//	函数原型：	同上
//  参数：      同上
//  返回值：    同上
//  用法：		同上
//*****************************************************************************
USHORT mqwUtils::bswap_16(ULONG value)
{
	_asm mov eax, value
	_asm and eax, 0xFFFF
	_asm xchg ah, al
}
