
#ifndef _BreadCriticalSection_h_
#define _BreadCriticalSection_h_

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

namespace Bread
{
	class cCriticalSection
	{
	public:
		cCriticalSection();
		~cCriticalSection();
		void initialize();
		void initializeAndSpinCount(DWORD count);
		void finalize();
		void lock();
		void unlock();
		void lock(TCHAR const* msg);
		void unlock(TCHAR const* msg);
		bool isLock();

	private:
		CRITICAL_SECTION m_cs;
	};
}








#endif