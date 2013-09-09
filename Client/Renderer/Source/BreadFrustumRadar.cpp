
#include "BreadFrustumRadar.h"

namespace Bread
{
	implementRTTI(cFrustumRadar, cFrustum)

	/*
	*/
	cFrustumRadar::cFrustumRadar()
	{
	}
	/*
	*/
	cFrustumRadar::~cFrustumRadar()
	{
	}
	/*
	*/
	void cFrustumRadar::setProjParam(float fov, float aspect, float znear, float zfar)
	{
		//m_rFactor = cMath::tanf(cMath::degToRad(fov/2.0f));
		m_rFactor = cMath::tanf(fov/2.0f);//cMath::degToRad(fov/2.0f));
		m_uFactor = m_rFactor * aspect;
		m_znear = znear;
		m_zfar = zfar;
	}
	/*
	*/
	void cFrustumRadar::construct(sMatrix4 const& view, sMatrix4 const& /*proj*/, sVector3 const& eye)
	{
		m_eye = eye;
		view.getA(m_rightVector);
		view.getB(m_upVector);
		view.getC(m_forwardVector);
	}
	/*
	*/
	bool cFrustumRadar::isVisible(sVector3 const& v) const
	{
		return isVisible(v, 0.0f);
	}
	/*
	*/
	bool cFrustumRadar::isVisible(sVector3 const* v, int v_num, bool& isEntireVisible) const
	{
		/// 우선 false로 해둔다
		isEntireVisible = true;
		bool visible = false;

		for (int i = 0; i < v_num; ++i)
		{
			/// 한개의 점이라도 절두체에 들어 있으면 모두 들어 있는 걸로 간주
			if (isVisible(v[i]))
			{
				visible = true;
			}
			else
			{
				isEntireVisible = false;
			}

		}

		return visible;
	}
	/*
	*/
	bool cFrustumRadar::isVisible(sVector3 const& center, float radius) const
	{
		sVector3 op;
		subVec3(center, m_eye, op);

		float f = m_forwardVector.dot(op);
		if (f < m_znear - radius || f > m_zfar + radius) 
			return false;

		float r = m_rightVector.dot(op);
		float rLimit = m_rFactor * f + radius;
		if (r < -rLimit || r > rLimit) 
			return false;

		float u = m_upVector.dot(op);
		float uLimit = m_uFactor * f + radius;
		if (u < -uLimit || u > uLimit) 
			return false;

		return true;

	}
	/*
	*/
	bool cFrustumRadar::isVisible(sCullFigure const& cullFigure, bool& isEntireVisible) const
	{
		if (!isVisible(cullFigure.m_sphere)) 
			return false;

		if (!isVisible(cullFigure.m_box, isEntireVisible)) 
			return false;

		return true;
	}
	/*
	*/
	bool cFrustumRadar::isVisible(sSphere const& sphere) const
	{
		return isVisible(sphere.m_center, sphere.m_radius);
	}
	/*
	*/
	bool cFrustumRadar::isVisible(sBox const& box, bool& isEntireVisible) const
	{
		return isVisible(box.m_v, sBox::VERT_NUM, isEntireVisible);
	}
}