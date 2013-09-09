
#include "BreadCriticalSection.h"
#include "BreadTrace.h"


namespace Bread
{
	/*
	 *	
	 */
	cCriticalSection::cCriticalSection()
	{
		ZeroMemory(&m_cs, sizeof (m_cs));
	}
	/*
	 *	
	 */
	cCriticalSection::~cCriticalSection()
	{
	}
	/*
	 *	
	 */
	void cCriticalSection::initialize()
	{
		InitializeCriticalSection(&m_cs);
	}
	/*
	 *	
	 */
	void cCriticalSection::finalize()
	{
		if (m_cs.OwningThread)
			DeleteCriticalSection(&m_cs);
	}
	/*
	 *	
	 */
	void cCriticalSection::initializeAndSpinCount(DWORD count)
	{
		InitializeCriticalSectionAndSpinCount(&m_cs, count);
	}
	/*
	 *	
	 */
	void cCriticalSection::lock()
	{
		EnterCriticalSection(&m_cs);
	}
	/*
	 *	
	 */
	void cCriticalSection::unlock()
	{
		LeaveCriticalSection(&m_cs);
	}
	/*
	 *	
	 */
	void cCriticalSection::lock(TCHAR const* msg)
	{
		EnterCriticalSection(&m_cs);
		trace(_T("%s\n"), msg);
	}
	/*
	 *	
	 */
	void cCriticalSection::unlock(TCHAR const* msg)
	{
		LeaveCriticalSection(&m_cs);
		trace(_T("%s\n"), msg);
	}
	/*
	 *	
	 */
	bool cCriticalSection::isLock()
	{
		return (m_cs.LockCount >= 0);
	}
}








