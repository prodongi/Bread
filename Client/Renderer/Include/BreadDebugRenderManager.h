
#ifndef _BreadDebugRenderManager_h_
#define _BreadDebugRenderManager_h_

#include "BreadDeclaration.h"
#include "BreadUiPrimitive.h"
#include "BreadBucketVector.h"

#define _getDebugRenderMgr() Bread::cDebugRenderManager::getInstance()

namespace Bread
{
	class cTexture;

	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	class cDebugRenderManager
	{
		declareInstance(cDebugRenderManager)
		declareClassCreater(cDebugRenderManager)

	public:
		virtual void initialize();
		virtual void finalize();
		virtual void render3D();
		virtual void render2D();
		void clearList();

		void addScreen(float sx, float sy, float w, float h, cTexture* tex); 

	protected:
		void renderScreen();

	protected:
		struct sScreen
		{
			float m_sx;
			float m_sy;
			float m_w;
			float m_h;
			cTexture* m_tex;
		};

	protected:
		cBucketVector<sScreen>	m_bucketScreen;
	};
}



#endif