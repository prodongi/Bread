
#ifndef _BreadCubeTexture_h_
#define _BreadCubeTexture_h_

#include "BreadTexture.h"

namespace Bread
{
	/*
	*/
	struct sCubeTexture : public sTexture
	{
		sCubeTexture() : m_tex(NULL) {}
		sCubeTexture(LPDIRECT3DCUBETEXTURE9 tex) : m_tex(tex)	{}
		virtual ~sCubeTexture()	{	safeRelease(m_tex);	}
		virtual bool load(std_string const& filename);
		virtual void getDesc(D3DSURFACE_DESC& desc);
		virtual LPDIRECT3DBASETEXTURE9 getTexture() const	{	return m_tex;	}
		void setTexture(LPDIRECT3DCUBETEXTURE9 tex)			{	m_tex = tex;	}
		
	protected:
		LPDIRECT3DCUBETEXTURE9 m_tex;
	};
}

#endif