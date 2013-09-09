
#ifndef _BreadTexture_h_
#define _BreadTexture_h_

#include "BreadTextureLoader.h"

namespace Bread
{
	#define TEXTURE_TYPE_2D				0
	#define TEXTURE_TYPE_CUBE			1
	#define TEXTURE_TYPE_VOLUME		2

	/*
	*/
	struct sTextureLockRect
	{
		int m_pitch;
		void* m_data;
	};
	/*
	*/
	struct sTexture
	{
		virtual ~sTexture() {}
		virtual bool load(std_string const& /*filename*/)			{	return false;	}
		virtual bool create(uint /*w*/, uint /*h*/, D3DFORMAT /*format*/)	{	return false;	}
		virtual LPDIRECT3DBASETEXTURE9 getTexture() const		{	return NULL;	}
		virtual bool lock(sTextureLockRect& /*lockRect*/)			{	return false;	}
		virtual bool unlock()											{	return false;	}
		virtual void getDesc(D3DSURFACE_DESC& /*desc*/)		{}
		virtual void getDesc(D3DVOLUME_DESC& /*desc*/)		{}
		/// @brief 텍스춰 데이타를 랜덤한 로테이션 값으로 채운다, 아직 값이 불안정 한것 같다.
		virtual void makeRandomleyRotateData(uint /*width*/, uint /*height*/)	{}
	};
	
	/*
	*/
	class cTexture : public cTextureLoader
	{
	public:
		cTexture();
		virtual ~cTexture();
		virtual bool loadTexture(std_string const& filename, int textureType);
		virtual bool createTexture(int textureType);
		virtual bool createTexture(int textureType, uint w, uint h, D3DFORMAT format);
		void setTexture(sTexture* tex);
		bool lock(sTextureLockRect& lockRect);
		bool unlock();

		LPDIRECT3DBASETEXTURE9 getTexture() const	{	return (m_texture) ? m_texture->getTexture() : NULL;	}
		uint getWidth() const					{	return m_width;		}
		uint getHeight() const					{	return m_height;	}
		bool is() const							{	return m_texture != NULL;	}
		cTexture* copyingThread()				{	return NULL;			}
		sTexture* getData()						{	return m_texture;	}

	protected:
		void getDesc();
		
	protected:
		sTexture* m_texture;
		uint m_width;
		uint m_height;
	};
}







#endif









