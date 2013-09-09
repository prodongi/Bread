
#ifndef _BreadLightManager_h_
#define _BreadLightManager_h_

#include "BreadBucketVector.h"
#include "BreadLight.h"

#define _getLightMgr()	Bread::cLightManager::getInstance()

namespace Bread
{
	class cLightManager : public cBucketVector<cLight*>
	{
		declareInstance(cLightManager)
		declareClassCreater(cLightManager)

	public:
		void update(float elapsedtime);
		int renderDebugInfo(int y);

	protected:
	};
}















#endif









