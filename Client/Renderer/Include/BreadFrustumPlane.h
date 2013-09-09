
#ifndef _BreadFrustumPlane_h_
#define _BreadFrustumPlane_h_

#include "BreadFrustum.h"

namespace Bread
{
	/*
		@date 2012.
		@auth prodongi
		@desc 
		@todo 
	*/
	class cFrustumPlane : public cFrustum
	{
		declareRTTI()

	public:
		cFrustumPlane();
		virtual ~cFrustumPlane();

		/// m_plane 생성
		virtual void construct(sMatrix4 const& view, sMatrix4 const& proj, sVector3 const& eye);

		/// @return in:false, out:true
		virtual bool isVisible(sVector3 const& v) const;
		virtual bool isVisible(sVector3 const* v, int v_num, bool& isEntireVisible) const;
		virtual bool isVisible(sVector3 const& center, float radius) const;
		virtual bool isVisible(sCullFigure const& cullFigure, bool& isEntireVisible) const;
		virtual bool isVisible(sSphere const& sphere) const;
		virtual bool isVisible(sBox const& box, bool& isEntireVisible) const;
		
	protected:
		enum	{	P_NEAR, P_FAR, P_LEFT, P_RIGHT, P_TOP, P_BOTTOM, P_NUM	};

		/// @brief 평면의 방향은 안쪽으로 향한다
		sPlane	m_plane[P_NUM];
	};
}



#endif