#pragma once

#include <type_traits>
#include <string>
#include <codecvt>
#include <boost/thread.hpp>

namespace saer
{
struct _c2t_cpp{};

template<typename S, typename D, typename T = _c2t_cpp>
class c2t
{
private:
	c2t<S,D,T>(const c2t<S,D,T>&) {}
	c2t<S,D,T>& operator=(const c2t<S,D,T>&) {return *this;}

public:
	//enable_if validate Source equal to Destination
	c2t(const std::basic_string<S>& in, typename std::enable_if<std::is_same<S, D>::value>::type* = 0) : m_data(in.c_str()) {}
	c2t(const S* in, typename std::enable_if<std::is_same<S, D>::value>::type* = 0) : m_data(in) {}
	~c2t() {}

	explicit operator const D*() { return m_data; }
	//In this operator we don't use referece because m_data is a pointer not an object
	operator const std::basic_string<D>() { return m_data; }

private:
	const S* m_data;
};

template<>
class c2t<char, wchar_t>
{
public:
	c2t(const std::string& in) { init(in.c_str()); }
	c2t(const char* in) { init(in); }
	~c2t(){}

	explicit operator const wchar_t*() { return m_data.data(); }
	operator const std::wstring&() { return m_data; }

private:
	void init(const char* in)
	{
		try
		{
			typedef std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
			convert converter;
			m_data = converter.from_bytes(in);
			return;
		}
		catch (std::range_error&)
		{
			//Some character is not UTF-8
		}
		//Second chance, now we use ansi
		try
		{
			typedef std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>, wchar_t> convert_ascii;
			convert_ascii converter;
			m_data = converter.from_bytes(in);
		}
		catch (std::range_error&)
		{
		}
	}

private:
	std::wstring m_data;
};


template<>
class c2t<wchar_t, char>
{
public:
	c2t(const std::wstring& in) { init(in.c_str()); }
	c2t(const wchar_t* in) { init(in); }
	~c2t(){}

	explicit operator const char*() { return m_data.data(); }
	operator const std::string&() { return m_data; }

private:
	void init(const wchar_t* in)
	{
		try
		{
			typedef std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
			convert converter;
			m_data = converter.to_bytes(in);
		}
		catch (std::range_error&)
		{
		}
	}

private:
	std::string m_data;
};

struct _c2t_native{};


template<>
class c2t<char, wchar_t, _c2t_native>
{
public:
	c2t(const std::string& in) { init(in.c_str()); }
	c2t(const char* in) { init(in); }
	~c2t(){}

	explicit operator const wchar_t*() { return m_data.data(); }
	operator const std::wstring&() { return m_data; }

private:
	void init(const char* in)
	{
		int l = (int)strlen(in);
		if (l + 1 < 100)
		{
			wchar_t utf16[100];

			int cchW = MultiByteToWideChar(CP_UTF8, 0, in, l, utf16, l + 1);
			if (cchW > 0)
				m_data = std::move(std::wstring(utf16, cchW));
			else
			{
				cchW = MultiByteToWideChar(CP_ACP, 0, in, l, utf16, l + 1);
				if (cchW > 0)
					m_data = std::move(std::wstring(utf16, cchW));
			}
		}
		else
		{
			wchar_t* utf16 = new wchar_t[l + 1];

			int cchW = MultiByteToWideChar(CP_UTF8, 0, in, l, utf16, l + 1);
			if (cchW > 0)
				m_data = std::move(std::wstring(utf16, cchW));
			else
			{
				cchW = MultiByteToWideChar(CP_ACP, 0, in, l, utf16, l + 1);
				if (cchW > 0)
					m_data = std::move(std::wstring(utf16, cchW));
			}

			delete[] utf16;
		}
	}

private:
	std::wstring m_data;
};



template<>
class c2t<wchar_t, char, _c2t_native>
{
public:
	c2t(const std::wstring& in) { init(in.c_str()); }
	c2t(const wchar_t* in) { init(in); }
	~c2t(){}

	explicit operator const char*() { return m_data.data(); }
	operator const std::string&() { return m_data; }

private:
	void init(const wchar_t* in)
	{
		int l = (int)wcslen(in);
		if (l * 3 + 1 < 100)
		{
			char utf8[100];

			int cch = WideCharToMultiByte(CP_UTF8, 0, in, l, utf8, l * 3 + 1, nullptr, false);
			if (cch > 0)
				m_data = std::move(std::string(utf8, cch));
		}
		else
		{
			char* utf8 = new char[l * 3 + 1];

			int cch = WideCharToMultiByte(CP_UTF8, 0, in, l, utf8, l * 3 + 1, nullptr, false);
			if (cch > 0)
				m_data = std::move(std::string(utf8, cch));

			delete[] utf8;
		}
	}

private:
	std::string m_data;
};

//Uso la versión nativa de win32 porque su rendimiento en msvc10 es muy superior

#ifdef UNICODE
	typedef c2t<char, wchar_t, _c2t_native> ca2ct;
	typedef c2t<wchar_t, char, _c2t_native> ct2ca;
    typedef c2t<wchar_t, wchar_t> cw2ct;
    typedef c2t<wchar_t, wchar_t> ct2cw;
#else
	typedef c2t<char, char> ca2ct;
	typedef c2t<char, char> ct2ca;
#endif
typedef  c2t<wchar_t, char, _c2t_native> cw2ca;
typedef  c2t<char, wchar_t, _c2t_native> ca2cw;

}
