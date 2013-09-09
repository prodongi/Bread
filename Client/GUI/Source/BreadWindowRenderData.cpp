
#include <tchar.h>
#include "BreadWindowRenderData.h"
#include "BreadRenderer.h"


namespace Bread
{
	/*
	 *	
	 */
	cWindowRenderData::cWindowRenderData() : m_color(0xffffffff)
	{
	}
	/*
	 *	
	 */
	cWindowRenderData::~cWindowRenderData()
	{
		m_vb.release();
	}
	/*
	 *	
	 */
	bool cWindowRenderData::createVB(cRect2 const& posRect, cRect2 const& texRect,
										float texWidth, float texHeight)
	{
		if (m_vb.is())
		{
			assert(0 && _T("m_vb isn't NULL"));
			return false;
		}
		if (cMath::equal(posRect.m_width, 1.0f))
			return false;
		if (cMath::equal(posRect.m_height, 1.0f))
			return false;

		sUiPlane plane;
		float u0 = texRect.m_pos.x/texWidth;
		float v0 = texRect.m_pos.y/texHeight;
		float u1 = (texRect.m_pos.x+texRect.m_width)/texWidth;
		float v1 = (texRect.m_pos.y+texRect.m_height)/texHeight;
		// 1 3
		// 0 2
		plane.m_vertex[0].set(sVector3(posRect.m_pos.x, posRect.m_pos.y + posRect.m_height, 0.0f), u0, v1, m_color);
		plane.m_vertex[1].set(sVector3(posRect.m_pos.x, posRect.m_pos.y, 0.0f), u0, v0, m_color);

		plane.m_vertex[2].set(sVector3(posRect.m_pos.x+posRect.m_width, posRect.m_pos.y + posRect.m_height, 0.0f), u1, v1, m_color);
		plane.m_vertex[3].set(sVector3(posRect.m_pos.x+posRect.m_width, posRect.m_pos.y, 0.0f), u1, v0, m_color);

		return cRenderer::createVertexBuffer(4 * sUiPlane::VERTEX_SIZE, sUiPlane::D3DFVF, plane.m_vertex, m_vb);
	}
	/*
	 *	
	 */
	void cWindowRenderData::render(LPDIRECT3DDEVICE9 device)
	{
		if (!m_vb.is()) return ;
		device->SetTexture(0, NULL);
		device->SetVertexShader(NULL);
		device->SetPixelShader(NULL);
		device->SetFVF(sUiPlane::D3DFVF);
		cRenderer::renderPrimitive(D3DPT_TRIANGLESTRIP, 0, 2, m_vb, sUiPlane::VERTEX_SIZE);
	}
}