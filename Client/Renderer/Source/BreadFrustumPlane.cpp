
#include "BreadFrustumPlane.h"
#include "BreadVector3.h"
#include "BreadRenderData.h"

namespace Bread
{
	implementRTTI(cFrustumPlane, cFrustum)

	/*
	*/
	cFrustumPlane::cFrustumPlane()
	{
	}
	/*
	*/
	cFrustumPlane::~cFrustumPlane()
	{
	}
	/*
	 *	
	 */
	void cFrustumPlane::construct(sMatrix4 const& view, sMatrix4 const& proj, sVector3 const& /*eye*/)
	{
		sMatrix4 matrix;
		mulMat4(view, proj, matrix);

		sPlane* plane;
		//near plane
		plane = m_plane + P_NEAR;
		plane->a = matrix.m[0][3] + matrix.m[0][2];
		plane->b = matrix.m[1][3] + matrix.m[1][2];
		plane->c = matrix.m[2][3] + matrix.m[2][2];
		plane->d = matrix.m[3][3] + matrix.m[3][2];
		plane->normalize();

		//far plane
		plane = m_plane + P_FAR;
		plane->a = matrix.m[0][3] - matrix.m[0][2];
		plane->b = matrix.m[1][3] - matrix.m[1][2];
		plane->c = matrix.m[2][3] - matrix.m[2][2];
		plane->d = matrix.m[3][3] - matrix.m[3][2];
		plane->normalize();

		//left plane 
		plane = m_plane + P_LEFT;
		plane->a = matrix.m[0][3] + matrix.m[0][0];
		plane->b = matrix.m[1][3] + matrix.m[1][0];
		plane->c = matrix.m[2][3] + matrix.m[2][0];
		plane->d = matrix.m[3][3] + matrix.m[3][0];
		plane->normalize();

		//right plane
		plane = m_plane + P_RIGHT;
		plane->a = matrix.m[0][3] - matrix.m[0][0];
		plane->b = matrix.m[1][3] - matrix.m[1][0];
		plane->c = matrix.m[2][3] - matrix.m[2][0];
		plane->d = matrix.m[3][3] - matrix.m[3][0];
		plane->normalize();

		//top plane
		plane = m_plane + P_TOP;
		plane->a = matrix.m[0][3] - matrix.m[0][1];
		plane->b = matrix.m[1][3] - matrix.m[1][1];
		plane->c = matrix.m[2][3] - matrix.m[2][1];
		plane->d = matrix.m[3][3] - matrix.m[3][1];
		plane->normalize();

		//bottom plane
		plane = m_plane + P_BOTTOM;
		plane->a = matrix.m[0][3] + matrix.m[0][1];
		plane->b = matrix.m[1][3] + matrix.m[1][1];
		plane->c = matrix.m[2][3] + matrix.m[2][1];
		plane->d = matrix.m[3][3] + matrix.m[3][1];
		plane->normalize();
	}
	/*
	 */
	bool cFrustumPlane::isVisible(sVector3 const& v) const
	{
		return isVisible(v, 0.0f);
	}
	/*
	 *	
	 */
	bool cFrustumPlane::isVisible(sVector3 const* v, int v_num, bool& isEntireVisible) const
	{
		isEntireVisible = true;

		int j;
		for (j = 0; j < P_NUM; ++j)
		{
			bool allOut = true;

			for(int i=0; i < v_num; ++i)
			{
				if(m_plane[j].dotCoord(v[i]) < 0.0f)
				{
					isEntireVisible = false;
				}
				else
				{
					allOut = false;
				}
			}

			if (allOut)
				return false;
		}

		return true;
	}
	/*
	 *	
	 */
	bool cFrustumPlane::isVisible(sVector3 const& center, float radius) const
	{
		for(int i=0; i < P_NUM; ++i)
		{
			if(m_plane[i].dotCoord(center) < -radius)
				return false;
		}
		return true;
	}
	/*
	 *	
	 */
	bool cFrustumPlane::isVisible(sCullFigure const& cullFigure, bool& isEntireVisible) const
	{
		// sphere
		if (!isVisible(cullFigure.m_sphere))
			return false;
		// box
		if (!isVisible(cullFigure.m_box, isEntireVisible))
			return false;
		return true;
	}
	/*
	 *	
	 */
	bool cFrustumPlane::isVisible(sSphere const& sphere) const
	{
		return isVisible(sphere.m_center, sphere.m_radius);
	}
	/*
	 *
	 */
	bool cFrustumPlane::isVisible(sBox const& box, bool& isEntireVisible) const
	{
		return isVisible(box.m_v, sBox::VERT_NUM, isEntireVisible);
	}
}



















