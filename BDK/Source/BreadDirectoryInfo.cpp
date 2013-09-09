
#include "BreadDirectoryInfo.h"
#include <assert.h>


namespace Bread
{
	/*
	 *	
	 */
	bool cDirectoryInfo::initialize()
	{
		if (0 == GetCurrentDirectory(MAX_PATH, m_directory))
		{
			assert(0 && _T("failed get currentdirectory"));
			return false;
		}

		return true;
	}
}