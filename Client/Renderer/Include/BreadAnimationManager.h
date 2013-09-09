
#ifndef _BreadAnimationManager_h_
#define _BreadAnimationManager_h_

#include "BreadBucketHash.h"
#include "BreadAnimation.h"

#define _getAnimationMgr() Bread::cAnimationManager::getInstance()

namespace Bread
{
	#define ANIMATIONMANAGER_BUCKET_SIZE	100

	class cEntityModel;
	class cRenderData;
	class cAnimationEventManager;
	class cAnimationActionManager;
	class cAnimationAction;

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
	 * \date 2009-12-03
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
	class cAnimationManager : public cBucketHash<BUID, cAnimation*, ANIMATIONMANAGER_BUCKET_SIZE>
	{
		declareInstance(cAnimationManager)
		declareClassCreater(cAnimationManager)

	public:
		virtual void initialize();
		void setMotion(uint id, cEntityModel* entity);
		void update(float elapsedtime);
		void renderBone();
		
	protected:
		cAnimationEventManager* m_eventMgr;
		cAnimationActionManager* m_actionMgr;
	};
}













#endif


















