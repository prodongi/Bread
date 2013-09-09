
#include "BreadRenderToTexture.h"
#include "BreadD3DSystemClassify.h"
#include "BreadCore.h"
#include "Bread2DTexture.h"

namespace Bread
{
	/*
	 *	
	 */
	cRenderToTextureBase::cRenderToTextureBase() : m_surface(NULL), m_depth(NULL), m_clearZ(1.0f)
	{
	}
	/*
	 *	
	 */
	cRenderToTextureBase::~cRenderToTextureBase()
	{
		safeRelease(m_surface);
		safeRelease(m_depth);
	}
	/*
	 *	
	 */
	void cRenderToTextureBase::setVp(uint width, uint height)
	{
		m_vp.X = 0;
		m_vp.Y = 0;
		m_vp.Width = (DWORD)width;
		m_vp.Height = (DWORD)height;
		m_vp.MinZ = 0.0f;
		m_vp.MaxZ = 1.0f;
	}
	/*
	 *	
	 */
	void cRenderToTextureBase::colorFill(D3DCOLOR color)
	{
		cD3DSystem::getD3DDevice()->ColorFill(m_surface, NULL, color);
	}
	/*
	 *	
	 */
	bool cRenderToTextureBase::lockRect(D3DSURFACE_DESC* desc, D3DLOCKED_RECT* lockedRect, RECT const* rect, DWORD flags)
	{
		if (!checkHResultReturn(m_surface->GetDesc(desc)))
			return false;
		if (!checkHResultReturn(m_surface->LockRect(lockedRect, rect, flags)))
			return false;

		return true;
	}
	/*
	 *	
	 */
	bool cRenderToTextureBase::unlockRect()
	{
		if (!checkHResultReturn(m_surface->UnlockRect()))
			return false;
		return true;
	}
	/*
	 *	
	 */
	void cRenderToTextureBase::setScreen(float sx, float sy, float w, float h, D3DCOLOR color)
	{
		m_screen.set(sx, sy, w, h, color);
	}







	/*
	 *	
	 */
	cRenderToTextureNAA::cRenderToTextureNAA() : m_texture(NULL)
	{
	}
	/*
	 *	
	 */
	cRenderToTextureNAA::~cRenderToTextureNAA()
	{
		safeDelete(m_texture);
	}
	/*
	 *	
	 */
	bool cRenderToTextureNAA::initialize(uint mrtIndex, uint width, uint height, bool makeDepth, 
										 D3DFORMAT texFormat, BOOL /*lockable*/)
	{
		this->setScreen(0.0f, 0.0f, (float)width, (float)height);
		m_mrtIndex = mrtIndex;
		LPDIRECT3DDEVICE9 device = cD3DSystem::getD3DDevice();
		if (makeDepth)
		{
			if (!checkHResultReturn(device->CreateDepthStencilSurface(width, height, _getD3DSystem()->getParameters()->AutoDepthStencilFormat,
													   D3DMULTISAMPLE_NONE, 0, TRUE, &m_depth, NULL)))
													   return false;
		}
		LPDIRECT3DTEXTURE9 tex;
		if (!checkHResultReturn(device->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, texFormat, D3DPOOL_DEFAULT, &tex, NULL)))
			return false;
		if (!checkHResultReturn(tex->GetSurfaceLevel(0, &m_surface)))
			return false;

		m_texture = new cTexture;
		m_texture->setTexture(new s2DTexture(tex));

		setVp(width, height);
		m_width = width;
		m_height = height;

		return true;
	}
	/*
	 *	
	 */
	bool cRenderToTextureNAA::begin(bool clear, DWORD color)
	{
		LPDIRECT3DDEVICE9 device = cD3DSystem::getD3DDevice();
		if (0 == m_mrtIndex)	
		{	
			if (!checkHResultReturn(device->GetRenderTarget(m_mrtIndex, &m_oldSurface)))
				return false;
			if (m_depth)	
			{	
				if (!checkHResultReturn(device->GetDepthStencilSurface(&m_oldDepth))	)
					return false;
			}
		}
		else
		{
			m_oldSurface = NULL;
		}
		
		if (!checkHResultReturn(device->GetViewport(&m_oldVp)))
			return false;

		if (!checkHResultReturn(device->SetRenderTarget(m_mrtIndex, m_surface)))
			return false;
		if (m_depth)	
		{	
			if (!checkHResultReturn(device->SetDepthStencilSurface(m_depth)))
				return false;
		}
		if (!checkHResultReturn(device->SetViewport(&m_vp)))
			return false;

		if (clear)
			device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, m_clearZ, 0L);
		return true;
	}
	/*
	 *	
	 */
	bool cRenderToTextureNAA::end()
	{
		LPDIRECT3DDEVICE9 device = cD3DSystem::getD3DDevice();
		if (0 == m_mrtIndex)
		{
			if (m_depth)
			{
				if (!checkHResultReturn(device->SetDepthStencilSurface(m_oldDepth)))
					return false;
				safeRelease(m_oldDepth);
			}
		}
		if (!checkHResultReturn(device->SetRenderTarget(m_mrtIndex, m_oldSurface)))
			return false;
		safeRelease(m_oldSurface);
		if (!checkHResultReturn(device->SetViewport(&m_oldVp)))
			return false;
		
		return true;
	}






	/*
	 *	
	 */
	cRenderToTextureMSAA::cRenderToTextureMSAA() : m_destTexture(NULL)
	{
	}
	/*
	 *	
	 */
	cRenderToTextureMSAA::~cRenderToTextureMSAA()
	{
		safeDelete(m_destTexture);
	}
	/*
	 *	
	 */
	bool cRenderToTextureMSAA::initialize(uint mrtIndex, uint width, uint height, bool makeDepth, 
										  D3DFORMAT texFormat, BOOL lockable)
	{
		this->setScreen(0.0f, 0.0f, (float)width, (float)height);
		m_mrtIndex = mrtIndex;
		LPDIRECT3DDEVICE9 device = cD3DSystem::getD3DDevice();

		D3DMULTISAMPLE_TYPE msType = D3DMULTISAMPLE_NONE;
		DWORD msQuaility = 0;
		if (FALSE == lockable)
		{
			msType = (D3DMULTISAMPLE_TYPE)_getEngineConfig()->getRttMultiSampleType();
			msQuaility = (DWORD)_getEngineConfig()->getRttMultiSampleQuality();
		}
		D3DFORMAT fmt = texFormat;
		if (_getEngineConfig()->getRttMultiSampleFloat16())
		{
			if (_getD3DSystem()->getRttMSFloat16Info(_getEngineConfig()->getRttMultiSampleType(), (uint&)msType, (uint&)msQuaility))
				fmt = D3DFMT_A16B16G16R16F;
		}

		if (makeDepth)
		{
			D3DFORMAT depthFmt = _getD3DSystem()->getParameters()->AutoDepthStencilFormat;
			if (!checkHResultReturn(device->CreateDepthStencilSurface(width, height, depthFmt, msType, msQuaility, TRUE, &m_depth, NULL)))
				return false;
		}
		if (!checkHResultReturn(device->CreateRenderTarget(width, height, fmt, msType, msQuaility, lockable, &m_surface, NULL)))
			return false;

		//
		LPDIRECT3DTEXTURE9 tex;
		if (!checkHResultReturn(device->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, fmt, D3DPOOL_DEFAULT, &tex, NULL)))
			return false;
		if (!checkHResultReturn(tex->GetSurfaceLevel(0, &m_destSurface)))
			return false;

		m_destTexture = new cTexture;
		m_destTexture->setTexture(new s2DTexture(tex));

		setVp(width, height);
		m_width = width;
		m_height = height;

		return true;
	}
	/*
	 *	
	 */
	bool cRenderToTextureMSAA::begin(bool clear, DWORD color)
	{
		LPDIRECT3DDEVICE9 device = cD3DSystem::getD3DDevice();
		if (0 == m_mrtIndex)
		{	
			if (!checkHResultReturn(device->GetRenderTarget(m_mrtIndex, &m_oldSurface)))
				return false;
			if (m_depth)	
			{	
				if (!checkHResultReturn(device->GetDepthStencilSurface(&m_oldDepth)))
					return false;
			}
		}
		else
		{
			m_oldSurface = NULL;
		}
		
		if (!checkHResultReturn(device->GetViewport(&m_oldVp)))
			return false;

		if (!checkHResultReturn(device->SetRenderTarget(m_mrtIndex, m_surface)))
			return false;
		if (m_depth)	
		{	
			if (!checkHResultReturn(device->SetDepthStencilSurface(m_depth)))
				return false;
		}
		if (!checkHResultReturn(device->SetViewport(&m_vp)))
			return false;

		if (clear)
			device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0L);
		return true;
	}
	/*
	 *	
	 */
	bool cRenderToTextureMSAA::end()
	{
		LPDIRECT3DDEVICE9 device = cD3DSystem::getD3DDevice();
		if (!checkHResultReturn(device->StretchRect(m_surface, NULL, m_destSurface, NULL, D3DTEXF_NONE)))
			return false;

		if (0 == m_mrtIndex)
		{
			if (m_depth)
			{
				if (!checkHResultReturn(device->SetDepthStencilSurface(m_oldDepth)))
					return false;
				safeRelease(m_oldDepth);
			}
		}
		if (!checkHResultReturn(device->SetRenderTarget(m_mrtIndex, m_oldSurface)))
			return false;
		safeRelease(m_oldSurface);
		if (!checkHResultReturn(device->SetViewport(&m_oldVp)))
			return false;
				
		return true;
	}
}