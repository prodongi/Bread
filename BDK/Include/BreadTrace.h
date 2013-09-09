
#ifndef _BreadTrace_h_
#define _BreadTrace_h_

#include <tchar.h>

namespace Bread
{
#pragma pack(1)
	/*
	*/
	struct sVerbosity
	{
		static int m_level;
	};
#pragma pack()
	void trace(TCHAR const* format, ...);
	void traceLastError();
	/// verbosity level로 출력 제어
	void traceVerbose(int verbosityLevel, TCHAR const* format);
}

#endif