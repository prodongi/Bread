
#include "BreadStringHelper.h"
#include <stdarg.h>
#include <windows.h>

namespace Bread
{
	implementSingleton(cStringHelper)

	/*
	*/
	cStringHelper::cStringHelper()
	{
	}
	/*
	*/
	TCHAR const* cStringHelper::make(TCHAR const* format, ...)
	{
		va_list vl;
		va_start(vl, format);
		_vstprintf(m_buffer, format, vl);
		va_end(vl);

		return m_buffer;
	}
	/*
	 *	
	 */
	TCHAR const* cStringHelper::makeFilenameToBUID(std_string const& filename)
	{
		if (filename.length() < 1)
			return _T("");

		static TCHAR _filename[NAME_LEN];
		static TCHAR _expname[NAME_LEN];
		_tsplitpath(filename.c_str(), NULL, NULL, _filename, _expname);

		TCHAR const* exp = (_tcslen(_expname) > 0) ? _expname+1 : NULL;
		return makeBUID(_filename, exp);
	}
	/*
	*	
	*/
	TCHAR const* cStringHelper::makeBUID(TCHAR const* param1, TCHAR const* param2)
	{
		static TCHAR retStr[MAX_PATH];
		if (param2)
			_stprintf(retStr, _T("%s.%s"), param1, param2);
		else
			_tcsncpy_s(retStr, param1, MAX_PATH);
		return retStr;
	}
	/*
	*/
	TCHAR const* cStringHelper::makeFilenameToNotExp(std_string const& filename)
	{
		if (filename.length() < 1)
			return _T("");
		_tsplitpath(filename.c_str(), NULL, NULL, m_buffer, NULL);

		return m_buffer;
	}
	/*
	*/
	TCHAR const* cStringHelper::toString(int i)
	{
		_stprintf(m_buffer, _T("%d"), i);
		return m_buffer;
	}
	/*
	*/
	TCHAR const* cStringHelper::toString(uint i)
	{
		_stprintf(m_buffer, _T("%ud"), i);
		return m_buffer;
	}
	/*
	*/
	void cStringHelper::split(std_string const& str, TCHAR const* seperator, std::vector<std_string>& strList)
	{
		strList.clear();

		int seperatorLen = _tcsclen(seperator);
		std_string::size_type spos, epos;
		spos = str.find_first_not_of(seperator);
		while ((epos = str.find_first_of(seperator, spos)) != std_string::npos)
		{
			strList.push_back(str.substr(spos, epos - spos));
			spos = str.find_first_not_of(seperator, epos+seperatorLen);
			if (spos == std_string::npos)
				break;
		}
		if (spos != std_string::npos)
			strList.push_back(str.substr(spos, str.length() - spos));
	}
	/*
	*/
	void cStringHelper::replace(std_string& str, TCHAR const* from, TCHAR const* to)
	{
		std_string::size_type p;
		while ((p = str.find(from)) != std_string::npos)
		{
			str.replace(p, 1, to);
		}
	}
#ifdef _UNICODE
	int cStringHelper::m2w(char const* str, TCHAR* tstr, int tstrSize, unsigned int codePage)
	{
		return MultiByteToWideChar(codePage, MB_PRECOMPOSED, str, -1, tstr, tstrSize);;
	}
	int cStringHelper::w2m(TCHAR const* tstr, char* str, int strSize, unsigned int codePage)
	{
		return WideCharToMultiByte(codePage, 0, tstr, -1, str, strSize, NULL, NULL);;
	}
#else
	char* cStringHelper::m2w(char const* str, TCHAR* tstr, int /*tstrsize*/, unsigned int /*codePage*/)
	{
		return strcpy(tstr, str);
	}
	char* cStringHelper::w2m(TCHAR const* tstr, char* str, int /*strsize*/, unsigned int /*codePage*/)
	{
		return strcpy(str, tstr);
	}
#endif
}