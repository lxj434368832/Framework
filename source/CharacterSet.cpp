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
