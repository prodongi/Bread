
#ifndef _BreadTextureManager_h_
#define _BreadTextureManager_h_

#include "BreadDeclaration.h"
#include "BreadTexture.h"

#define _getTextureMgr() Bread::cTextureManager::getInstance()

namespace Bread
{
	class cTextureCache;

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
	 * \date 2009-10-03
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
	class cTextureManager
	{
		declareInstance(cTextureManager)

	public:
		cTextureManager();
		virtual ~cTextureManager();
		virtual bool initialize();
		virtual void finalize();
		virtual void update(float elapsedtime);
		virtual void mergeThread();

		bool loadTexture(std_string const& filename, int textureType = TEXTURE_TYPE_2D, int loadingType = LOADING_BACKGROUND);
		cTexture* createTexture(BUID const& buid, int textureType);
		cTexture* createTexture(BUID const& buid, int textureType, uint w, uint h, D3DFORMAT format);
		void setTexture(uint stage, BUID const& buid);
		cTexture* getTexture(BUID const& buid);
		cTexture* getDefault();
		cTexture* get(BUID const& buid);

	protected:
		cTextureCache* m_cache;
	};
}









#endif