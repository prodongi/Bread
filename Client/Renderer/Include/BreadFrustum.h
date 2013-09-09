
#ifndef _BreadFrustum_h_
#define _BreadFrustum_h_

#include "BreadClientDefinition.h"
#include "BreadMatrix4.h"
#include "BreadPlane.h"

namespace Bread
{
	/**
	 * \ingroup Renderer
	 *
	 *
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2009-09-12
	 *
	 * \author prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo http://3DApi.com 에서 참조
	 *
	 * \bug 
	 *
	 */
	class cFrustum
	{
		declareRTTI()

	public:
		cFrustum();
		virtual ~cFrustum();

		virtual void setProjParam(float /*fov*/, float /*aspect*/, float /*znear*/, float /*zfar*/) {}
		/// m_plane 생성
		virtual void construct(sMatrix4 const& /*view*/, sMatrix4 const& /*proj*/, sVector3 const& /*eye*/) {}

		virtual bool isVisible(sVector3 const& /*v*/) const	{	return false;	}
		virtual bool isVisible(sVector3 const* /*v*/, int /*v_num*/, bool& /*isEntireVisible*/) const	{	return false;	}
		virtual bool isVisible(sVector3 const& /*center*/, float /*radius*/) const { return false;	}
		virtual bool isVisible(sCullFigure const& /*cullFigure*/, bool& /*isEntireVisible*/) const	{	return false;	}
		virtual bool isVisible(sSphere const& /*sphere*/) const	{	return false;	}
		virtual bool isVisible(sBox const& /*box*/, bool& /*isEntireVisible*/) const { return false; }
	};
}


#endif






