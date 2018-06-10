#pragma once

#include <string>
#include <codecvt>
#include <locale>

namespace saer
{
template<typename S, typename D>
class c2t
{
private:
	c2t<S,D>(const c2t<S,D>&) {}
	c2t<S,D>& operator=(const c2t<S,D>&) {return *this;}
	
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
class c2t<char16_t, char>
{
public:
	c2t(const std::u16string& in) {  init(in.c_str()); }
	c2t(const char16_t* in) { init(in); }
	~c2t(){}

	explicit operator const char*() { return m_data.data(); }
	operator const std::string&() { return m_data; }

private:
	void init(const char16_t* in)
	{
		try
		{
			std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> convert;
			m_data = convert.to_bytes(in);
		}
		catch(const std::range_error&)
		{
		}
	}

private:
	std::string m_data;
};


template<>
class c2t<char, char16_t>
{
public:
	c2t(const std::string& in) { init(in.c_str()); }
	c2t(const char* in) { init(in); }
	~c2t(){}

	explicit operator const char16_t*() { return m_data.data(); }
	operator const std::u16string&() { return m_data; }

private:
	void init(const char* in)
	{
		try
		{
			std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> convert;
			m_data = convert.from_bytes(in);
		}
		catch(const std::range_error&)
		{
		}
	}

private:
	std::u16string m_data;
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
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
			m_data = convert.to_bytes(in);
		}
		catch(const std::range_error&)
		{
		}
	}

private:
	std::string m_data;
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
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
			m_data = convert.from_bytes(in);
		}
		catch(const std::range_error&)
		{
		}
	}

private:
        std::wstring m_data;
};

typedef c2t<char, char> ca2ct;
typedef c2t<char, char> ct2ca;

typedef c2t<char16_t, char> cu16_2ca;
typedef c2t<char, char16_t> ca2cu16;

typedef c2t<wchar_t, char> cw2ct;
typedef c2t<char, wchar_t> ct2cw;


typedef c2t<wchar_t, char> cw2ca;
typedef  c2t<char, wchar_t> ca2cw;
}
