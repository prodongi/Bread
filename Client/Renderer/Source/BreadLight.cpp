
#include "BreadLight.h"
#include "BreadDeclaration.h"
#include "BreadCore.h"
#include "BreadCameraManager.h"
#include "BreadRenderData.h"
#include "BreadEntityManager.h"
#include "BreadD3DUtil.h"
#include "BreadStringHelper.h"
#include "BreadGui.h"


namespace Bread
{
	/*
	 *
	 */
	void cLight::determineScissorRect(RECT* rect) const
	{
		if (D3DLIGHT_DIRECTIONAL == m_type)
		{
			SetRect(rect, 0, 0, _getEngineConfig()->getWidth(), _getEngineConfig()->getHeight());
			return ;
		}

		cCamera* cam = _getCameraMgr()->getMain();

		sBox box;
		box.set(m_position.x-m_range, m_position.x+m_range,
				m_position.y-m_range, m_position.y+m_range,
				m_position.z-m_range, m_position.z+m_range);

		uint minx = UINT_MAX;
		uint maxx = 0;
		uint miny = UINT_MAX;
		uint maxy = 0;
		uint x, y;

		sVector2 _2d;
		for (int i = 0; i < sBox::VERT_NUM; ++i)
		{
			cam->convert3DTo2D(box.m_v[i], _2d, true);

			x = (uint)_2d.x;
			y = (uint)_2d.y;

			if (x < minx)	minx = x;
			if (x > maxx)	maxx = x;
			if (y < miny)	miny = y;
			if (y > maxy)	maxy = y;
		}

		SetRect(rect, minx, miny, maxx, maxy);
	}
	/*
	 *	
	 */
	void cLight::calculMatVP()
	{
		m_matView.makeViewMatrixDir(m_position, m_direction);
		//m_matProj.makeOrthoMatrix(1024.0f, 768.0f, 2.0f, 10000.0f);
		m_matProj.makePerspectiveMatrix(0.78f, 1.33f, 1.0f, 10000.0f);
		mulMat4(m_matView, m_matProj, m_matVP);
	}
	/*
	 *	
	 */
	void cLight::setPosition(sVector3 const& pos)
	{
		m_position = pos;
		calculMatVP();
		syncEntityPos();
	}
	/*
	 *	
	 */
	void cLight::setDirection(sVector3 const& dir)
	{
		m_direction = dir;
		calculMatVP();
	}
	/*
	 *	
	 */
	void cLight::syncEntityPos()
	{
		if (m_entity.empty())
			return ;
		cEntityModel* entity = _getEntityMgr()->get(m_entity);
		if (!entity)
			return ;
		entity->setPosition(m_position);
	}
	/*
	*/
	int cLight::renderDebugInfo(int y)
	{
		TCHAR const* str = _getStringHelper()->make(_T("pos:%.2f, %.2f, %.2f"), m_position.x, m_position.y, m_position.z);
		_getFontMgr()->drawD3DText(10, y+=16, str, cD3DFont::HEIGHT_14, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		return y;
	}
}