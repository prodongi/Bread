
#include "BreadGrid.h"
#include "BreadCore.h"
#include "BreadRenderPropertyManager.h"
#include "BreadRenderPrimitive.h"
#include "Bread2DTexture.h"

namespace Bread
{
	/*
	*/
	cGrid::cGrid() : m_tex(NULL)
	{
	}
	/*
	*/
	cGrid::~cGrid()
	{
		safeDelete(m_tex);
	}
	/*
	 *	
	 */
	void cGrid::initialize()
	{
		assert(!m_tex);

		m_tex = new s2DTexture;
		if (!m_tex->load(_T("BaseData\\grid.tga")))
			return ;
	}
	/*
	 *	
	 */
	void cGrid::render()
	{
		if (!_getIsRender(RENDER_GRID))
			return ;

		D3DSURFACE_DESC desc;
		m_tex->getDesc(desc);
		float size = 10000.0f;
		float tu = size/(float)desc.Width;
		float tv = size/(float)desc.Height;

		sFaceVertex v[4];
		D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);
		
		// 0 2
		// 1 3
		v[0].set(-size, 0.0f, size, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, color);
		v[1].set(-size, 0.0f, -size, 0.0f, 1.0f, 0.0f, 0.0f, tv, color);
		v[2].set(size, 0.0f, size, 0.0f, 1.0f, 0.0f,   tu, 0.0f, color);
		v[3].set(size, 0.0f, -size, 0.0f, 1.0f, 0.0f,  tu, tv, color);
		
		LPDIRECT3DDEVICE9 device = cD3DSystem::getD3DDevice();
	
		_getRenderPropertyMgr()->begin();
		_getRenderPropertyMgr()->setProperty(sRenderProperty::LIGHTING, FALSE);

		device->SetTexture(0, m_tex->getTexture());
		device->SetVertexShader(NULL);
		device->SetPixelShader(NULL);
		device->SetFVF(sFaceVertex::D3DFVF);

		cRenderer::renderPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (void*)v, sFaceVertex::SIZE);

		_getRenderPropertyMgr()->end();
	}
}
