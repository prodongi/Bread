
#ifndef _BreadDirectoryInfo_h_
#define _BreadDirectoryInfo_h_

#include <tchar.h>
#include <windows.h>

namespace Bread
{
	/**
	 * \ingroup Core
	 *
	 *
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2010-04-05
	 *
	 * \author Prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo 프로젝트 디렉토리 정보
	 *
	 * \bug 
	 *
	 */
	class cDirectoryInfo
	{
	public:
		cDirectoryInfo() {}
		~cDirectoryInfo() {}
		bool initialize();
		TCHAR const* getDirectory() const	{	return m_directory;	}

	private:
		TCHAR m_directory[MAX_PATH];
	};

}





#endif