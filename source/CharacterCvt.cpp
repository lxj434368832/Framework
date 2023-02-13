#include "../include/CharacterCvt.h"
#ifdef _WIN32
#include <stringapiset.h>
#include <locale>
#include <codecvt>
#else
#include <locale>
#include <codecvt>
#endif

BEGIN_NAMESPACE //namespace is mqwUtils

//实用程序包装器，以适应语言环境绑定的方面，以进行wstring / wbuffer转换
template<class Facet>
struct deletable_facet : Facet
{
	template<class... Args>
	deletable_facet(Args&&... args) : Facet(std::forward<Args>(args)...) {}
	~deletable_facet() {}
};

template<class Facet>
struct deletable_namefacet : Facet
{
    deletable_namefacet():Facet(""){}
    template<class... Args>
    deletable_namefacet(Args&&... args) : Facet(std::forward<Args>(args)...) {}
    ~deletable_namefacet() {}
};

std::string WideStringToMBString(LPCWSTR lpwStr, size_t szLen/* = -1*/)
{
    std::string mbStr;
#ifdef _WIN32
    /*int nCount = ::WideCharToMultiByte(CP_ACP, 0, lpwStr, (int)szLen, nullptr, 0, nullptr, nullptr);
    if (nCount < 1) return mbStr;
    mbStr.resize((size_t)nCount);
    ::WideCharToMultiByte(CP_ACP, 0, lpwStr, (int)szLen, &mbStr[0], nCount, nullptr, nullptr);*/
	typedef std::codecvt_byname<wchar_t, char, std::mbstate_t> Facet;
	std::wstring_convert<Facet> cvt(new Facet(""));
	try {
		mbStr = cvt.to_bytes(lpwStr);
	}
	catch (const std::range_error& e) {
		mbStr = cvt.to_bytes(lpwStr, lpwStr + cvt.converted());
	}
#else
    /*#ifdef __linux__
            setlocale(LC_ALL, "");
        #endif
            size_t szCount = wcstombs(nullptr, lpwStr, 0);
            if (0 == szCount || (size_t)-1 == szCount) return mbStr;
            mbStr.resize(szCount);
            szCount = wcstombs(&mbStr[0], lpwStr, szCount);*/
    std::wstring_convert<deletable_namefacet<
        std::codecvt_byname<wchar_t, char, std::mbstate_t>>> cvt("");
    try {
        mbStr = cvt.to_bytes(lpwStr);
    }
    catch (const std::range_error& e) {
        mbStr = cvt.to_bytes(lpwStr, lpwStr + cvt.converted());
    }
#endif
    return mbStr;
}

class default_codecvt : public std::codecvt_byname<wchar_t, char, std::mbstate_t>
{
public:
	default_codecvt() :codecvt_byname("") {}
	~default_codecvt() = default;
};

#ifdef __linux__
#include<iconv.h>
//to_charset 不支持“”和“char”
template<class charT>
std::basic_string<charT> iconv_convert(char* from_charset, char* to_charset, char* inbuf, size_t inLen)
{
	std::basic_string<charT> out;

	constexpr size_t szBufLen = 512;
	char buf[szBufLen] = {}, * outbuf = buf;
	size_t outLeft = szBufLen, szConv = 0;

	iconv_t cd = iconv_open(to_charset, from_charset);
	if ((iconv_t)-1 == cd) return out;
	while (inLen)
	{
		if ((size_t)-1 == iconv(cd, &inbuf, &inLen, &outbuf, &outLeft))
		{
			if (E2BIG == errno) { //"The output buffer has no more room for the next converted character."
				szConv = (szBufLen - outLeft) / sizeof(charT);
				out.append(buf, szConv);
				outbuf = buf;
				outLeft = szBufLen;
			}
			else if (EILSEQ == errno) { //"the input is invalid multibyte sequence"
				break;
			}
			else if (EINVAL == errno) { //"An incomplete multibyte sequence is encountered in the input"
				break;
			}
			else { //"convert failed"
				break;
			}
		}
		else {
			szConv = (szBufLen - outLeft) / sizeof(charT);
			out.append(buf, szConv);
		}
	}
	iconv_close(cd);
	return out;
}
#endif

static std::string UTF16ToUTF8_1(const char16_t* lpSrc, size_t szLen);
static std::string UTF32ToUTF8_1(const char32_t* lpSrc, size_t szLen);
std::string WideStringToUTF8String(LPCWSTR lpwStr, size_t szLen /*= -1*/)
{
    //以下为改良的ksbase方案
    /*if (0 == szLen) return std::string();
    if (sizeof(wchar_t) == sizeof(char16_t))
        return UTF16ToUTF8_1((const char16_t*)lpwStr, szLen);
    return UTF32ToUTF8_1((const char32_t*)lpwStr, szLen);*/

    //以下为改造的新方案
    std::string utf8Str;
#ifdef _WIN32
    int nCount = ::WideCharToMultiByte(CP_UTF8, 0, lpwStr, (int)szLen, nullptr, 0, nullptr, nullptr);
    if (nCount < 1) return utf8Str;
    utf8Str.resize((size_t)nCount);
    ::WideCharToMultiByte(CP_UTF8, 0, lpwStr, (int)szLen, &utf8Str[0], nCount, nullptr, nullptr);
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cvt;
	try {
		utf8Str = cvt.to_bytes(lpwStr);
	}
	catch (const std::range_error& e) {
		utf8Str = cvt.to_bytes(lpwStr, lpwStr + cvt.converted());
	}
	//以下为iconv方案
//    utf8Str = iconv_convert<char>((char*)"wchar_t", (char*)"UTF-8",(char*)lpwStr, szLen * sizeof(wchar_t));
#endif
    return utf8Str;
}

std::wstring MBStringToWideString(LPCSTR lpStr, size_t szLen /*= -1*/)
{
    std::wstring wStr;
#ifdef _WIN32
    int nCount = ::MultiByteToWideChar(CP_ACP, 0, lpStr, (int)szLen, nullptr, 0);
    if (0 == nCount) return wStr;
    wStr.resize((size_t)nCount);
    ::MultiByteToWideChar(CP_ACP, 0, lpStr, (int)szLen, &wStr[0], nCount);
#else
	std::wstring_convert<deletable_namefacet<
		std::codecvt_byname<wchar_t, char, std::mbstate_t>>> cvt("");
	try {
		wStr = cvt.from_bytes(lpStr);
	}
	catch (const std::range_error& e) {
		wStr = cvt.from_bytes(lpStr, lpStr + cvt.converted());
	}
#endif
    return wStr;
}

std::wstring UTF8StringToWideString(LPCSTR lpStr, size_t szLen /*= -1*/)
{
	std::wstring wStr;
#ifdef _WIN32
	int nCount = ::MultiByteToWideChar(CP_UTF8, 0, lpStr, (int)szLen, nullptr, 0);
	if (0 == nCount) return wStr;
	wStr.resize((size_t)nCount);
	::MultiByteToWideChar(CP_UTF8, 0, lpStr, (int)szLen, &wStr[0], nCount);
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cvt;
	try {
		wStr = cvt.from_bytes(lpStr);
	}
	catch (const std::range_error& e) {
		wStr = cvt.from_bytes(lpStr, lpStr + cvt.converted());
	}
#endif
	return wStr;
}

#ifdef _WIN32
std::string UTF8ToWinAnsi(LPCSTR lpStr, size_t szLen /*= -1*/)
{
	std::string mbStr;
    std::wstring wStr;
	int nCount = ::MultiByteToWideChar(CP_UTF8, 0, lpStr, (int)szLen, nullptr, 0);
	if (0 == nCount) return mbStr;
	wStr.resize((size_t)nCount);
	::MultiByteToWideChar(CP_UTF8, 0, lpStr, (int)szLen, &wStr[0], nCount);

	nCount = ::WideCharToMultiByte(CP_ACP, 0, wStr.c_str(), (int)wStr.length()
        , nullptr, 0, nullptr, nullptr);
	mbStr.resize((size_t)nCount);
	::WideCharToMultiByte(CP_ACP, 0, wStr.c_str(), (int)wStr.length()
        , &mbStr[0], nCount, nullptr, nullptr);
    return mbStr;
}

std::string WinAnsiToUTF8(LPCSTR lpStr, size_t szLen /*= -1*/)
{
	std::string mbStr;
	std::wstring wStr;
	int nCount = ::MultiByteToWideChar(CP_ACP, 0, lpStr, (int)szLen, nullptr, 0);
	if (0 == nCount) return mbStr;
	wStr.resize((size_t)nCount);
	::MultiByteToWideChar(CP_ACP, 0, lpStr, (int)szLen, &wStr[0], nCount);

	nCount = ::WideCharToMultiByte(CP_UTF8, 0, wStr.c_str(), (int)wStr.length()
		, nullptr, 0, nullptr, nullptr);
	mbStr.resize((size_t)nCount);
	::WideCharToMultiByte(CP_UTF8, 0, wStr.c_str(), (int)wStr.length()
		, &mbStr[0], nCount, nullptr, nullptr);
	return mbStr;
}
#endif

static int UCS4TUTF8(unsigned int ucs4, char* pDst)
{
	int nBytes = 1;
	if (ucs4 < 0x80)
	{
		*pDst = (char)ucs4;
		return nBytes;
	}

	nBytes = 7;
	if (ucs4 < 0x800)
		nBytes = 2;
	else if (ucs4 < 0x10000)
		nBytes = 3;
	else if (ucs4 < 0x200000)
		nBytes = 4;
	else if (ucs4 < 0x4000000)
		nBytes = 5;
	else if (ucs4 < 0x80000000)
		nBytes = 6;
	static const int prefix[] = { 0, 0, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
	switch (nBytes)
	{
	case 7:
		pDst[6] = (char)((ucs4 & 0x3F) | 0x80); ucs4 >>= 6;
	case 6:
		pDst[5] = (char)((ucs4 & 0x3F) | 0x80); ucs4 >>= 6;
	case 5:
		pDst[4] = (char)((ucs4 & 0x3F) | 0x80); ucs4 >>= 6;
	case 4:
		pDst[3] = (char)((ucs4 & 0x3F) | 0x80); ucs4 >>= 6;
	case 3:
		pDst[2] = (char)((ucs4 & 0x3F) | 0x80); ucs4 >>= 6;
	case 2:
		pDst[1] = (char)((ucs4 & 0x3F) | 0x80); ucs4 >>= 6;
	case 1:
		pDst[0] = (char)(ucs4 | prefix[nBytes]);
	default:
		break;
	}
	return nBytes;
}

static std::string UTF16ToUTF8(const char16_t* lpSrc, size_t szLen)
{
    std::string strUTF8;
    strUTF8.reserve(szLen * 3);
    char dst[8];
    int nBytes = 0;
    unsigned int ucs4 = 0, w1 = 0, w2 = 0;
    for (size_t i = 0; i < szLen; ++i)
    {
        w1 = *lpSrc++;
        if (w1 < 0xD800 || w1 > 0xDFFF)
        {
            ucs4 = w1;
        }
        else if (i + 1 < szLen)
        {
            w2 = *lpSrc++;
            ucs4 = (w2 & 0x03FF) + (((w1 & 0x03FF) + 0x40) << 10);
            ++i;
        }
        else break;
        nBytes = UCS4TUTF8(ucs4, dst);
        strUTF8.append(dst, nBytes);
    }
    return strUTF8;
}

static std::string UTF32ToUTF8(const char32_t* lpSrc, size_t szLen)
{
    std::string strUTF8;
    strUTF8.reserve(szLen * 3);
    char dst[8];
    int nBytes = 0;;
    for (size_t i = 0; i < szLen; ++i)
    {
        nBytes = UCS4TUTF8(*lpSrc++, dst);
        strUTF8.append(dst, nBytes);
    }
    return strUTF8;
}

static void UCS4TUTF8_1(unsigned int ucs4, char* &pDst)
{
	int nBytes = 1;
	if (ucs4 < 0x80)
	{
		*pDst++ = (char)ucs4;
		return;
	}

	nBytes = 7;
	if (ucs4 < 0x800)
		nBytes = 2;
	else if (ucs4 < 0x10000)
		nBytes = 3;
	else if (ucs4 < 0x200000)
		nBytes = 4;
	else if (ucs4 < 0x4000000)
		nBytes = 5;
	else if (ucs4 < 0x80000000)
		nBytes = 6;
	static const int prefix[] = { 0, 0, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
	switch (nBytes)
	{
	case 7:
		pDst[6] = (char)((ucs4 & 0x3F) | 0x80); ucs4 >>= 6;
	case 6:
		pDst[5] = (char)((ucs4 & 0x3F) | 0x80); ucs4 >>= 6;
	case 5:
		pDst[4] = (char)((ucs4 & 0x3F) | 0x80); ucs4 >>= 6;
	case 4:
		pDst[3] = (char)((ucs4 & 0x3F) | 0x80); ucs4 >>= 6;
	case 3:
		pDst[2] = (char)((ucs4 & 0x3F) | 0x80); ucs4 >>= 6;
	case 2:
		pDst[1] = (char)((ucs4 & 0x3F) | 0x80); ucs4 >>= 6;
	case 1:
		pDst[0] = (char)(ucs4 | prefix[nBytes]);
	default:
		break;
	}
	pDst += nBytes;
}

static std::string UTF16ToUTF8_1(const char16_t* lpSrc, size_t szLen)
{
	std::string strUTF8;
	strUTF8.reserve(szLen * 3);
	constexpr int bufCount = 512;
	char cBuf[bufCount] = {};
	char *pStart = &cBuf[0], * pDst = pStart;
	int nBytes = 0;
	unsigned int ucs4 = 0, w1 = 0, w2 = 0;
	for (size_t i = 0; i < szLen; ++i)
	{
		w1 = *lpSrc++;
		if (w1 < 0xD800 || w1 > 0xDFFF)
		{
			ucs4 = w1;
		}
		else if (i + 1 < szLen)
		{
			w2 = *lpSrc++;
			ucs4 = (w2 & 0x03FF) + (((w1 & 0x03FF) + 0x40) << 10);
			++i;
		}
		else break;
		UCS4TUTF8_1(ucs4, pDst);
		nBytes = pDst - pStart;
		if (nBytes > bufCount - 7)
		{
			strUTF8.append(pStart, nBytes);
			nBytes = 0;
			pDst = pStart;
		}
	}
	if(nBytes > 0) 
		strUTF8.append(pStart, nBytes);
	return strUTF8;
}

static std::string UTF32ToUTF8_1(const char32_t* lpSrc, size_t szLen)
{
	std::string strUTF8;
	strUTF8.reserve(szLen * 3);
	constexpr int bufCount = 512;
	char cBuf[bufCount] = {};
	char* pStart = &cBuf[0], * pDst = pStart;
	int nBytes = 0;;
	for (size_t i = 0; i < szLen; ++i)
	{
		UCS4TUTF8_1(*lpSrc++, pDst);
		nBytes = pDst - pStart;
		if (nBytes > bufCount - 7)
		{
			strUTF8.append(pStart, nBytes);
			nBytes = 0;
			pDst = pStart;
		}
	}
	if (nBytes > 0)
		strUTF8.append(pStart, nBytes);
	return strUTF8;
}


/*platform判断
#ifdef _WIN32
   //define something for Windows (32-bit and 64-bit, this part is common)
   #ifdef _WIN64
	  //define something for Windows (64-bit only)
   #else
	  //define something for Windows (32-bit only)
   #endif
#elif __APPLE__
	#if TARGET_IPHONE_SIMULATOR
		 // iOS Simulator
	#elif TARGET_OS_IPHONE
		// iOS device
	#elif TARGET_OS_MAC
		// Other kinds of Mac OS
#elif __ANDROID__
	// android
#elif __linux__
	// linux
#elif __unix__ // all unices not caught above
	// Unix
#elif defined(_POSIX_VERSION)
	// POSIX
#else
#   error "Unknown"
#endif
*/

END_NAMESPACE