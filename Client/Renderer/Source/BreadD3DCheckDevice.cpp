
#include "BreadD3DCheckDevice.h"


namespace Bread
{
	
	/*
	 *
	 */
	void sCheckDevice::query(LPDIRECT3D9 d3d, LPDIRECT3DDEVICE9 device, D3DFORMAT adapterFmt, D3DFORMAT backBufferFmt)
	{
		D3DCAPS9 caps;
		device->GetDeviceCaps(&caps);

		//
		if (caps.VertexShaderVersion < D3DVS_VERSION(2, 0))	m_vs = false;
		else												m_vs = true;
		//
		if (caps.PixelShaderVersion < D3DPS_VERSION(2, 0))	m_ps = false;
		else												m_ps = true;
		//
		if (caps.NumSimultaneousRTs < 4)	m_mrt = false;
		else								m_mrt = true;
		//
		if (FAILED(d3d->CheckDeviceFormat(caps.AdapterOrdinal, caps.DeviceType, backBufferFmt, 
										  D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, D3DRTYPE_TEXTURE, backBufferFmt)))
										m_postPixelShaderBlending = false;
		else							m_postPixelShaderBlending = true;
		//
		if (FAILED(d3d->CheckDeviceFormat(caps.AdapterOrdinal, caps.DeviceType, adapterFmt, D3DUSAGE_RENDERTARGET, 
										D3DRTYPE_CUBETEXTURE, D3DFMT_R32F)))
										m_rttR32F = false;
		else							m_rttR32F = true;
		//
		if (FAILED(d3d->CheckDeviceFormat(caps.AdapterOrdinal, caps.DeviceType, adapterFmt, D3DUSAGE_RENDERTARGET, 
										D3DRTYPE_CUBETEXTURE, D3DFMT_A8R8G8B8)))
										m_rttA8R8G8B8 = false;
		else							m_rttA8R8G8B8 = true;
		//
		if (caps.StencilCaps & D3DSTENCILCAPS_TWOSIDED)	m_stencil2Side = true;
		else											m_stencil2Side = false;
	}
}
















