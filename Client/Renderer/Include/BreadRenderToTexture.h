
#ifndef _BreadRenderToTexture_h_
#define _BreadRenderToTexture_h_

#include "BreadTexture.h"
#include "BreadUiPrimitive.h"

namespace Bread
{
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
	 * \date 2009-12-31
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
	class cRenderToTextureBase
	{
	public:
		cRenderToTextureBase();
		virtual ~cRenderToTextureBase();
		virtual bool initialize(uint mrtIndex, uint width, uint height, bool makeDepth = true, D3DFORMAT texFormat = D3DFMT_A8R8G8B8, BOOL lockable = FALSE) = 0;
		virtual bool begin(bool clear, DWORD color = 0) = 0;
		virtual bool end() = 0;
		virtual cTexture* getTexture() const = 0;
		void colorFill(D3DCOLOR color);
		bool lockRect(D3DSURFACE_DESC* desc, D3DLOCKED_RECT* lockedRect, RECT const* rect = NULL, DWORD flags = D3DLOCK_READONLY);
		bool unlockRect();
		LPDIRECT3DSURFACE9 getSurface() const {	return m_surface;	}
		LPDIRECT3DSURFACE9 getDepth()	const {	return m_depth;		}
		uint getWidth() const	{	return m_width;		}
		uint getHeight() const	{	return m_height;	}
		void setClearZ(float z)	{	m_clearZ = z;		}
		sUiPlane const* getScreen() const	{	return &m_screen;	}

	protected:
		void setVp(uint width, uint height);
		void setScreen(float sx, float sy, float w, float h, D3DCOLOR color = 0xffffffff);
		
	protected:
		LPDIRECT3DSURFACE9 m_surface;
		LPDIRECT3DSURFACE9 m_depth;
		D3DVIEWPORT9 m_vp;

		LPDIRECT3DSURFACE9 m_oldSurface;
		LPDIRECT3DSURFACE9 m_oldDepth;
		D3DVIEWPORT9 m_oldVp;

		uint m_width;
		uint m_height;
		uint m_mrtIndex;
		float m_clearZ;
		sUiPlane m_screen;
	};
	/*
	 *	none anti aliasing
	 */
	class cRenderToTextureNAA : public cRenderToTextureBase
	{
	public:
		cRenderToTextureNAA();
		virtual ~cRenderToTextureNAA();
		virtual bool initialize(uint mrtIndex, uint width, uint height, bool makeDepth = true, D3DFORMAT texFormat = D3DFMT_A8R8G8B8, BOOL lockable = FALSE);
		virtual bool begin(bool clear, DWORD color = 0);
		virtual bool end();
		virtual cTexture* getTexture()	const {	return m_texture;	}

	protected:
		cTexture* m_texture;
	};
	/*
	 *	multi sample anti aliasing
	 */
	class cRenderToTextureMSAA : public cRenderToTextureBase
	{
	public:
		cRenderToTextureMSAA();
		virtual ~cRenderToTextureMSAA();
		virtual bool initialize(uint mrtIndex, uint width, uint height, bool makeDepth = true, D3DFORMAT texFormat = D3DFMT_A8R8G8B8, BOOL loackable = FALSE);
		virtual bool begin(bool clear, DWORD color = 0);
		virtual bool end();
		virtual cTexture* getTexture()	const {	return m_destTexture;	}

	protected:
		cTexture* m_destTexture;
		LPDIRECT3DSURFACE9 m_destSurface;
	};
}








#endif














