
#ifndef _BreadD3DCheckDevice_h_
#define _BreadD3DCheckDevice_h_

#include "BreadD3DHeader.h"


namespace Bread
{
	/*
	 * Check Device State
	 */
	struct sCheckDevice
	{
		void query(LPDIRECT3D9 d3d, LPDIRECT3DDEVICE9 device, D3DFORMAT adapterFmt, D3DFORMAT backBufferFmt);

		bool m_vs;
		bool m_ps;
		bool m_mrt;
		bool m_postPixelShaderBlending;
		bool m_rttR32F;
		bool m_rttA8R8G8B8;
		bool m_stencil2Side;
	};
}












#endif