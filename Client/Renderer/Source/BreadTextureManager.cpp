
#include "BreadTextureManager.h"
#include "BreadTextureCache.h"
#include "BreadD3DSystemClassify.h"
#include "BreadTexture.h"
#include "BreadCubeTexture.h"

namespace Bread
{
	implementInstance(cTextureManager)

	/*
	*	
	*/
	cTextureManager::cTextureManager() : m_cache(NULL)
	{
		assignInstance(this)
	}
	/*
	*	
	*/
	cTextureManager::~cTextureManager()
	{
		assignInstance(NULL)
		safeDelete(m_cache);
	}
	/*
	*/
	bool cTextureManager::initialize()
	{
		assert(!m_cache);

		m_cache = new cTextureCache;
		m_cache->initialize();
		
		loadTexture(_T("Basedata\\default.jpg"), TEXTURE_TYPE_2D, LOADING_DIRECT);

		return true;
	}
	/*
	*/
	void cTextureManager::finalize()
	{
		safeDelete(m_cache, true);
	}
	/*
	*/
	void cTextureManager::update(float elapsedtime)
	{
		m_cache->update(elapsedtime);
	}
	/*
	*/
	void cTextureManager::mergeThread()
	{
		m_cache->mergeThread();
	}
	
	/*
	*/
	bool cTextureManager::loadTexture(std_string const& filename, int textureType, int loadingType)
	{
		BUID buid = cTexture::makeBUID(filename);
		if (m_cache->is(buid))
			return true;

		if (LOADING_BACKGROUND == loadingType)
		{
			m_cache->addLoadingList(sTextureLoadInfo(buid, filename, textureType, loadingType));
		}
		else
		{
			cTexture* texture = m_cache->create();
			if (!texture)
				return false;
			if (!texture->initialize(filename, textureType, loadingType))
				return false;

			m_cache->insert(texture->getBuid(), texture);
			if (LOADING_DIRECT == loadingType)
				texture->loadTexture(filename, textureType);
		}
		return true;
	}
	/*
	*/
	cTexture* cTextureManager::createTexture(BUID const& buid, int textureType)
	{
		cTexture* tex = get(buid);
		if (tex)
			return tex;

		cTexture* texture = m_cache->create();
		if (!texture->createTexture(textureType))
		{
			safeDelete(texture);
			return NULL;
		}
		m_cache->insert(buid, texture);

		return texture;
	}
	/*
	*/
	cTexture* cTextureManager::createTexture(BUID const& buid, int textureType, uint w, uint h, D3DFORMAT format)
	{
		cTexture* tex = get(buid);
		if (tex)
			return tex;

		cTexture* texture = m_cache->create();
		if (!texture->createTexture(textureType, w, h, format))
		{
			safeDelete(texture);
			return NULL;
		}
		m_cache->insert(buid, texture);

		return texture;
	}
	/*
	 *	
	 */
	void cTextureManager::setTexture(uint stage, BUID const& buid)
	{
		cTexture* texture = m_cache->get(buid);
		if (!texture)
			return ;

		if (!texture->is() && LOADING_ACCESS == texture->getLoadingType())
		{
			if (texture->loadTexture(texture->getFileName(), TEXTURE_TYPE_2D))
			{
				cD3DSystem::getD3DDevice()->SetTexture(0, NULL);
				return ;
			}
		}

		cD3DSystem::getD3DDevice()->SetTexture(stage, texture->getTexture());
	}
	/*
	 *	
	 */
	cTexture* cTextureManager::getTexture(BUID const& buid)
	{
		cTexture* tex = get(buid);
		if (!tex)
		{
			return getDefault();
		}
		return tex;
	}
	/*
	*/
	cTexture* cTextureManager::getDefault()
	{
		cTexture* tex = m_cache->get(_T("default.jpg"));
		if (!tex)
		{
			return NULL;
		}
		return tex;
	}
	/*
	*/
	cTexture* cTextureManager::get(BUID const& buid)
	{
		return m_cache->get(buid);
	}
}















