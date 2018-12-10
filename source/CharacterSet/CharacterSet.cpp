#include "../../include/CharacterSet/CharacterSet.h"

wchar_t * mqwUtils::ANSIToUnicode(const char* str)
{
	int textlen;
	wchar_t * result;
	textlen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	result = (wchar_t *)malloc((textlen + 1)*sizeof(wchar_t));
	memset(result, 0, (textlen + 1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)result, textlen);
	return result;
}

char * mqwUtils::UnicodeToANSI(const wchar_t* str)
{
	char* result;
	int textlen;
	textlen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	result = (char *)malloc((textlen + 1)*sizeof(char));
	memset(result, 0, sizeof(char) * (textlen + 1));
	WideCharToMultiByte(CP_ACP, 0, str, -1, result, textlen, NULL, NULL);
	return result;
}

wchar_t * mqwUtils::UTF8ToUnicode(const char* str)
{
	int textlen;
	wchar_t * result;
	textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	result = (wchar_t *)malloc((textlen + 1)*sizeof(wchar_t));
	memset(result, 0, (textlen + 1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
	return result;
}

char * mqwUtils::UnicodeToUTF8(const wchar_t* str)
{
	char* result;
	int textlen;
	textlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	result = (char *)malloc((textlen + 1)*sizeof(char));
	memset(result, 0, sizeof(char) * (textlen + 1));
	WideCharToMultiByte(CP_UTF8, 0, str, -1, result, textlen, NULL, NULL);
	return result;
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

wchar_t* mqwUtils::m2w(const char* mbs)
{
	int len;
	wchar_t* buf;
	len = mbstowcs(NULL, mbs, 0);
	if (len == 0)
		return NULL;
	buf = (wchar_t *)malloc(sizeof(wchar_t)*(len + 1));
	memset(buf, 0, sizeof(wchar_t) *(len + 1));
	len = mbstowcs(buf, mbs, len + 1);
	return buf;
}

char* mqwUtils::ANSIToUTF8(const char* str)
{
	wchar_t * buf = ANSIToUnicode(str);
	char * ret = UnicodeToUTF8(buf);
	free(buf);
	return ret;
}

void mqwUtils::ANSIToUTF8(std::string& str)
{
	wchar_t * buf = ANSIToUnicode(str.c_str());
	char * ret = UnicodeToUTF8(buf);
	free(buf);
	str = ret;
}

char* mqwUtils::UTF8ToANSI(const char* str)
{
	wchar_t * buf = UTF8ToUnicode(str);
	char * ret = UnicodeToANSI(buf);
	free(buf);
	return ret;
}

void mqwUtils::UTF8ToANSI(std::string& str)
{
	wchar_t * buf = UTF8ToUnicode(str.c_str());
	char * ret = UnicodeToANSI(buf);
	str = ret;
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
