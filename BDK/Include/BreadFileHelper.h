
#ifndef _BreadFileHelper_h_
#define _BreadFileHelper_h_

#include <windows.h>
#include <tchar.h>
#include "BreadDefinition.h"

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
	 * \date 2010-03-23
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
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	class cFileHelper
	{
	public:
		cFileHelper() {}
		~cFileHelper() {}

		static bool is(std_string const& filename);
		static uint getFileSize(std_string const& filename);
		static char const* getFileNameInFullPass(char const* fullPass);
		static char const* getPassNameInFullPass(char const* fullPass);
		static TCHAR const* getFileNameInFullPassT(TCHAR const* fullPass);
		static TCHAR const* getPassNameInFullPassT(TCHAR const* fullPass);
		static void createDirectory(TCHAR const* dirName);
		static void getFileList(TCHAR const* directory, TCHAR const* exe, std::vector<std_string>& fileList, bool isSubDir = false);
		static void getSubDirList(TCHAR const* directory, std::vector<std_string>& dirList);

	private:
		static TCHAR m_tchar[MAX_PATH];
		static char m_char[MAX_PATH];
	};
}








#endif