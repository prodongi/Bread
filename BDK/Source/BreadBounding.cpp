
#include "BreadBounding.h"
#include "BreadMatrix4.h"

namespace Bread
{
	/*
	*/
	void sBox::setTranslation(sBox const& box, sVector3 const& pos, sMatrix4 const& matRot)
	{
		for (int i = 0; i < VERT_NUM; ++i)
		{
			matRot.transformCoord(box.m_v[i], m_v[i]);
			m_v[i] += pos;
		}
		align();
	}

	/*
	*/
	void sCullFigure::setTranslation(sVector3 const& pos, sMatrix4 const& matRot)
	{
		m_sphere.setPosition(m_sphereBk, pos);
		m_box.setTranslation(m_boxBk, pos, matRot);
	}
}