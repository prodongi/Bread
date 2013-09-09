
#include "BreadTrace.h"
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <float.h>

namespace Bread
{
	int sVerbosity::m_level = 5;
	/*
	*	
	*/
	void trace(TCHAR const* format, ...)
	{
		static TCHAR buffer[1024];
		va_list vl;
		va_start(vl, format);
		_vsntprintf(buffer, sizeof (buffer) - 1, format, vl);
		va_end(vl);

		OutputDebugString(buffer);
	}
	/*
	*/
	void traceLastError()
	{
		DWORD error = GetLastError();
		LPVOID msg;
		DWORD ret = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
								  FORMAT_MESSAGE_FROM_SYSTEM |
								  FORMAT_MESSAGE_IGNORE_INSERTS,
								  NULL,
								  error,
								  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
								  (LPTSTR)&msg,
								  0,
								  NULL);
		if (0 == ret)
		{
			trace(_T("Failed GetLastError"));
			return ;
		}
		trace((LPTSTR)msg);
	}
	/*
	*/
	void traceVerbose(int verbosityLevel, TCHAR const* format)
	{
		if (sVerbosity::m_level >= verbosityLevel)
			trace(format);
	}
}