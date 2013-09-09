
#ifndef _BreadStringHelper_h_
#define _BreadStringHelper_h_

#include <tchar.h>
#include "BreadSingleton.h"
#include "BreadDefinition.h"

namespace Bread
{
	/*
		@date 2010.08.06
		@auth prodongi
		@desc String Helper
		@todo 
	*/
	class cStringHelper
	{
		declareSingleton(cStringHelper)

	public:
		~cStringHelper() {}
		TCHAR const* make(TCHAR const* format, ...);
		TCHAR const* makeFilenameToBUID(std_string const& filename);
		TCHAR const* makeBUID(TCHAR const* param1, TCHAR const* param2);
		TCHAR const* makeFilenameToNotExp(std_string const& filename);
		TCHAR const* toString(int i);
		TCHAR const* toString(uint i);
		void split(std_string const& str, TCHAR const* seperator, std::vector<std_string>& strList);
		void replace(std_string& str, TCHAR const* from, TCHAR const* to);

#ifdef _UNICODE
		int m2w(char const* str, TCHAR* tstr, int tstrSize, unsigned int codePage = 949);
		int w2m(TCHAR const* tstr, char* str, int strSize, unsigned int codePage = 949);
#else
		char* m2w(char const* str, TCHAR* tstr, int tstrSize, unsigned int codePage = 949);
		char* w2m(TCHAR const* tstr, char* str, int strSize, unsigned int codePage = 949);
#endif

	private:
		TCHAR m_buffer[1024];
	};
}







#endif