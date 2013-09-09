
#ifndef _BreadScreenShot_h_
#define _BreadScreenShot_h_

#include "BreadDeclaration.h"

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
	class cScreenShot
	{
	public:
		cScreenShot();
		~cScreenShot();
		void initialize();
		void update();

	private:
		void capture();
		void getCurrentCount();
		int calculCount(TCHAR const* fileName);
		void saveToFile(TCHAR const* fileName);

		static TCHAR const* getExpName();

	public:
		static uint expType;
		static std_string directoryName;
		static unsigned char captureKey;

	private:
		int m_count;
	};
}










#endif