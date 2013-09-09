
#include "BreadTexture.h"
#include "Bread2DTexture.h"
#include "BreadCubeTexture.h"
#include "BreadVolumeTexture.h"
#include "BreadD3DSystemClassify.h"
#include "BreadUtils.h"
#include "BreadStringHelper.h"
#include "BreadFileSystem.h"

namespace Bread
{
	/*
	*/
	cTexture::cTexture() : m_texture(NULL), m_width(0), m_height(0)
	{
	}
	/*
	*/
	cTexture::~cTexture()
	{
		safeDelete(m_texture);
	}
	/*
	*/
	bool cTexture::loadTexture(std_string const& filename, int textureType)
	{
		if (m_texture)
		{
			assert(0 && _T("failed load texture for already exist"));
			return false;
		}

		createTexture(textureType);
		if (!m_texture->load(filename))
			return false;
		getDesc();

		return true;
	}
	/*
	*/
	bool cTexture::createTexture(int textureType)
	{
		switch (textureType)
		{
		case TEXTURE_TYPE_2D:		m_texture = new s2DTexture;			return true;
		case TEXTURE_TYPE_CUBE:		m_texture = new sCubeTexture;		return true;
		case TEXTURE_TYPE_VOLUME:	m_texture = new sVolumeTexture;	return true;
		}
		return false;
	}
	/*
	*/
	bool cTexture::createTexture(int textureType, uint w, uint h, D3DFORMAT format)
	{
		if (!createTexture(textureType))
			return false;

		m_width = w;
		m_height = h;
		return m_texture->create(w, h, format);
	}
	/*
	 *	
	 */
	void cTexture::setTexture(sTexture* tex)
	{
		safeDelete(m_texture);
		m_texture = tex;
		if (!tex) return ;

		getDesc();
	}
	/*
	*/
	void cTexture::getDesc()
	{
		if (!m_texture)
			return ;

		D3DSURFACE_DESC desc;
		m_texture->getDesc(desc);
		m_width = desc.Width;
		m_height = desc.Height;
	}
	/*
	*/
	bool cTexture::lock(sTextureLockRect& lockRect)
	{
		return m_texture->lock(lockRect);
	}
	/*
	*/
	bool cTexture::unlock()
	{
		return m_texture->unlock();
	}
}