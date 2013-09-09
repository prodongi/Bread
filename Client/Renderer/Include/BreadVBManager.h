
#ifndef _BreadVBManager_h_
#define _BreadVBManager_h_

#include "BreadBucketHash.h"
#include "BreadVBInfo.h"

#define _getVBMgr()	Bread::cVBManager::getInstance()

namespace Bread
{	
	#define VBMANAGER_BUCKET_SIZE	1000

	/**
	 * \ingroup Renderer
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
	 * \date 2009-10-01
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
	class cVBManager : public cBucketHash<BUID, sVBInfo*, VBMANAGER_BUCKET_SIZE>
	{
		declareInstance(cVBManager)

	public:
		cVBManager();
		virtual ~cVBManager();
		bool create(sVBInfo* vbInfo, char const* buffer);
		/// @brief create with no lock
		bool create(sVBInfo* vbInfo);

		static TCHAR const* makeBUID(TCHAR const* param1, TCHAR const* param2, uint identity);
	};
}









#endif