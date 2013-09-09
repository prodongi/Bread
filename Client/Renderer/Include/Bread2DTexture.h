
#ifndef _Bread2DTexture_h_
#define _Bread2DTexture_h_

#include "BreadTexture.h"

namespace Bread
{
	/*
	*/
	struct s2DTexture : public sTexture
	{
		s2DTexture() : m_tex(NULL) {}
		s2DTexture(LPDIRECT3DTEXTURE9 tex) : m_tex(tex)	{}
		virtual ~s2DTexture()	{	safeRelease(m_tex);	}
		virtual bool load(std_string const& filename);
		virtual bool create(uint w, uint h, D3DFORMAT format);
		virtual void getDesc(D3DSURFACE_DESC& desc);
		virtual bool lock(sTextureLockRect& lockRect);
		virtual bool unlock();
		virtual void makeRandomleyRotateData(uint width, uint height);
		virtual LPDIRECT3DBASETEXTURE9 getTexture() const	{	return m_tex;	}
		void setTexture(LPDIRECT3DTEXTURE9 tex)				{	m_tex = tex;	}

	protected:
		LPDIRECT3DTEXTURE9 m_tex;
	};
}

#endif