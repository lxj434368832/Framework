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

	typedef std::codecvt<wchar_t, char, mbstate_t> convert_ansi;
	int res = std::use_facet<convert_ansi>(loc).out(
		state,wTemp.data(), wTemp.data() + wTemp.size(), ws_next,
			buff.data(), buff.data() + buff.size(), sz_next);

	if (convert_ansi::ok == res)
	{
		return std::string(buff.data(), sz_next);
	}
	return "";
}

const std::string charset::UnicodeToANSI(const std::wstring& src)
{
	std::locale sys_locale("");

	const wchar_t* data_from = src.c_str();
	const wchar_t* data_from_end = src.c_str() + src.size();
	const wchar_t* data_from_next = 0;

	int wchar_size = 4;
	char* data_to = new char[(src.size() + 1) * wchar_size];
	char* data_to_end = data_to + (src.size() + 1) * wchar_size;
	char* data_to_next = 0;

	memset(data_to, 0, (src.size() + 1) * wchar_size);

	typedef std::codecvt<wchar_t, char, mbstate_t> codecvt_ansi;
	mbstate_t out_state = { 0 };
	auto result = std::use_facet<codecvt_ansi>(sys_locale).out(
		out_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next);

	if (result == codecvt_ansi::ok)
	{
		std::string dst = data_to;
		delete[] data_to;
		return dst;
	}
	else
	{
		delete[] data_to;
		return std::string("");
	}
}

const std::wstring charset::ANSIToUnicode(const std::string& src)
{
	std::locale sys_locale("");

	const char* data_from = src.c_str();
	const char* data_from_end = src.c_str() + src.size();
	const char* data_from_next = nullptr;

	wchar_t* data_to = new wchar_t[src.size() + 1];
	wchar_t* data_to_end = data_to + src.size() + 1;
	wchar_t* data_to_next = nullptr;

	wmemset(data_to, 0, src.size() + 1);

	typedef std::codecvt<wchar_t, char, mbstate_t> codecvt_ansi;
	mbstate_t in_state = { 0 };
	auto result = std::use_facet<codecvt_ansi>(sys_locale).in(
		in_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next);
	if (result == codecvt_ansi::ok)
	{
		std::wstring dst = data_to;
		delete[] data_to;
		return dst;
	}
	else
	{
		delete[] data_to;
		return std::wstring(L"");
	}
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

