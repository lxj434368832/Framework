#include "../include/charset.h"
#include <vector>
#include <codecvt>
#include <iostream>



// ANSI转UTF-8(c++11)        亦可使用s_to_ws()配合ws_to_utf8()
const std::string charset::ans_to_utf8(std::string const &a_str)
{
	std::vector<wchar_t> buff(a_str.size());
#ifdef _MSC_VER
	std::locale loc("");//"zh-CN"
#else
	std::locale loc("zh_CN.GB18030");
#endif
	wchar_t* ws_next = nullptr;
	const char* sz_next = nullptr;
	mbstate_t state = {};
	typedef std::codecvt<wchar_t, char, mbstate_t> convert_ansi;
	int res = std::use_facet<convert_ansi>(loc).in(
		state, a_str.data(), a_str.data() + a_str.size(), sz_next,
			buff.data(), buff.data() + buff.size(), ws_next);

	if (convert_ansi::ok == res)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
		return cutf8.to_bytes(std::wstring(buff.data(), ws_next));
	}

	return "";
}

// UTF-8转ANSI(c++11)
const std::string charset::utf8_to_ans(const std::string &u_str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
	std::wstring wTemp = cutf8.from_bytes(u_str);
#ifdef _MSC_VER
	std::locale loc("zh-CN");
#else
	std::locale loc("zh_CN.GB18030");
#endif
	const wchar_t* ws_next = nullptr;
	char* sz_next = nullptr;
	mbstate_t state = {};

	std::vector<char> buff(wTemp.size() * 2);

	const std::codecvt<wchar_t, char, std::mbstate_t>& cvt =
		std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);

	int res = cvt.out(state,wTemp.data(), wTemp.data() + wTemp.size(), ws_next,
			buff.data(), buff.data() + buff.size(), sz_next);

	if (std::codecvt_base::ok == res)
	{
		return std::string(buff.data(), sz_next);
	}
	return "";
}

const std::string charset::UnicodeToANSI(const wchar_t* strUnicode)
{
	std::locale sys_locale("");

	const size_t iFromSize = wcslen(strUnicode);
	const wchar_t* data_from = strUnicode;
	const wchar_t* data_from_end = strUnicode + iFromSize;
	const wchar_t* data_from_next = nullptr;

	const size_t iUnicodeLen = 4;
	int iToSize = iFromSize * iUnicodeLen + 1;
	std::vector<char> vct_data_to(iToSize);
	char* data_to = vct_data_to.data();
	char* data_to_end = data_to + iToSize;
	char* data_to_next = nullptr;

	const std::codecvt<wchar_t, char, std::mbstate_t>& cvt =
		std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(sys_locale);

	mbstate_t out_state = { 0 };
	auto result = cvt.out(out_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next);

	std::string strANSI = vct_data_to.data();
	if (result != std::codecvt_base::ok)
	{
		std::cerr << "UnicodeToANSI转换失败！";
	}
	return strANSI;
}

const std::wstring charset::ANSIToUnicode(const char* strANSI)
{
//#ifdef _MSC_VER
//	std::locale loc("zh-CN");
//#else
//	std::locale loc("zh_CN.GB18030");
//#endif
	std::locale loc("");
	mbstate_t state = { 0 };

	const size_t iFromSize = strlen(strANSI);
	const char* data_from = strANSI;
	const char* data_from_end = strANSI + iFromSize;
	const char* data_from_next = nullptr;

	const std::codecvt<wchar_t, char, std::mbstate_t>& cvt =
		std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);

	int iToSize = cvt.length(state, data_from, data_from_end, iFromSize);
	std::wstring strUnicode(iToSize, 0);
	wchar_t* data_to = (wchar_t*)strUnicode.data();
	wchar_t* data_to_end = data_to + iToSize;
	wchar_t* data_to_next = nullptr;

	auto result = cvt.in(state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next);
	if (result != std::codecvt_base::ok)
	{
		std::cerr << "ANSIToUnicode转换失败！";
	}
	return strUnicode;
}

const std::string charset::UnicodeToUTF8(const std::wstring& wstr)
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

const std::wstring charset::UTF8ToUnicode(const std::string& str)
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

