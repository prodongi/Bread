
#ifndef _BreadVolumeTexture_h_
#define _BreadVolumeTexture_h_

#include "BreadTexture.h"

namespace Bread
{
	/*
	*/
	struct sVolumeTexture : public sTexture
	{
		sVolumeTexture();
		sVolumeTexture(LPDIRECT3DVOLUMETEXTURE9 tex) : m_tex(tex) {}
		virtual ~sVolumeTexture()	{	safeRelease(m_tex);	}
		virtual bool load(std_string const& filename);
		virtual void getDesc(D3DVOLUME_DESC& desc);
		virtual LPDIRECT3DBASETEXTURE9 getTexture() const	{	return m_tex;	}
		void setTexture(LPDIRECT3DVOLUMETEXTURE9 tex)		{	m_tex = tex;	}
		/// @brief noise 값으로 텍스춰를 채운다.
		bool fillNoise();
		bool create(int width, int height, int depth, D3DFORMAT format);

	protected:
		LPDIRECT3DVOLUMETEXTURE9 m_tex;
	};
}

#endif