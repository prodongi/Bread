
#ifndef _BreadFrustumRadar_h_
#define _BreadFrustumRadar_h_

#include "BreadFrustum.h"
#include "BreadVector3.h"

namespace Bread
{
	/*
		@date 2012.
		@auth prodongi
		@desc 레이다가 컬링 하는 방식을 이용
		@todo cFrustumPlane보다 느리고, 체크되는 영역이 너무 작은 것 같다.
	*/
	class cFrustumRadar : public cFrustum
	{
		declareRTTI()

	public:
		cFrustumRadar();
		virtual ~cFrustumRadar();

		virtual void setProjParam(float fov, float aspect, float znear, float zfar);
		virtual void construct(sMatrix4 const& view, sMatrix4 const& proj, sVector3 const& /*eye*/);

		/// @return in:false, out:true
		virtual bool isVisible(sVector3 const& v) const;
		virtual bool isVisible(sVector3 const* v, int v_num, bool& isEntireVisible) const;
		virtual bool isVisible(sVector3 const& center, float radius) const;
		virtual bool isVisible(sCullFigure const& cullFigure, bool& isEntireVisible) const;
		virtual bool isVisible(sSphere const& sphere) const;
		virtual bool isVisible(sBox const& box, bool& isEntireVisible) const;
		
	protected:
		sVector3 m_eye;
		sVector3 m_forwardVector;
		sVector3 m_rightVector;
		sVector3 m_upVector;
		float m_rFactor;
		float m_uFactor;
		float m_znear;
		float m_zfar;
	};
}



#endif