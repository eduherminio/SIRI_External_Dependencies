#pragma once

#include <string>
#include <string.h>
#include <uchar.h>
#include <wchar.h>
#include <cwchar>

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
		std::mbstate_t state = std::mbstate_t();
		std::string out(MB_CUR_MAX, '\0');
		for(size_t n = 0; in[n] != 0; ++n)
		{   
			size_t rc = c16rtomb(&out[0], in[n], &state);
                        if ((rc==0)||(rc>MB_CUR_MAX)) break;
                        for(size_t x = 0; x < rc; ++x) 
                                m_data += out[x];
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
		char16_t c16;
		const char *ptr = in, *end = in + strlen(in);
		std::mbstate_t state = std::mbstate_t();
		while(size_t rc = mbrtoc16(&c16, ptr, end - ptr, &state))
		{   
			if(rc == -3)
			{
			}
			else if(rc > 0) {
				m_data += c16;
				ptr += rc;
			}
			else
				break; //error
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
      std::mbstate_t state = std::mbstate_t();
      int len = 1 + std::wcsrtombs(NULL, &in, 0, &state);
      if(len > 0)
      {
        m_data.resize(len, '\0');
        std::wcsrtombs(&m_data[0], &in, m_data.size(), &state);
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
      std::mbstate_t state = std::mbstate_t();
      int len = 1 + std::mbsrtowcs(NULL, &in, 0, &state);
      if(len > 0)
      {
        m_data.resize(len,L'\0');
        std::mbsrtowcs(&m_data[0], &in, m_data.size(), &state);
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
