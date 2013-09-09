
#include "BreadAABB.h"
#include "BreadRay.h"
#include "BreadPlane.h"
#include "BreadMath.h"

namespace Bread
{
	/*
	 *	
	 */
	void cAABB::set(const sVector3& min, const sVector3& max)
	{
		m_min = min;
		m_max = max;
	}
	/*
	 *	
	 */
	bool cAABB::collision(const cAABB& other) const
	{
		if (m_min.x > other.m_max.x || m_max.x < other.m_min.x)
			return false;
		if (m_min.y > other.m_max.y || m_max.y < other.m_min.y)
			return false;
		if (m_min.z > other.m_max.z || m_max.z < other.m_min.z)
			return false;

		return true;
	}
	/*
	 *	
	 */
	bool cAABB::collision(const sRay& ray) const
	{
		float maxS = -FLT_MAX;
		float minT = FLT_MAX;

		for (int i = 0; i < 3; ++i)
		{
			if (cMath::isZero(ray.m_dir[i]))
			{
				if (ray.m_orig[i] < m_min[i] || ray.m_orig[i] > m_max[i])
					return false;
			}
			else
			{
				float s = (m_min[i] - ray.m_orig[i])/ray.m_dir[i];
				float t = (m_max[i] - ray.m_orig[i])/ray.m_dir[i];

				if (s > t)
				{
					float temp = t;
					t = s;
					s = temp;
				}

				if (s > maxS)
					maxS = s;
				if (t < minT)
					minT = t;

				if (minT < 0.0f || maxS > minT)
					return false;
			}
		}

		return true;
	}
	/*
	 *	
	 */
	float cAABB::collision(const sPlane& plane) const
	{
		sVector3 diagMin, diagMax;

		for (int i = 0; i < 3; ++i)
		{
			if (plane[i] >= 0.0f)
			{
				diagMin[i] = m_min[i];
				diagMax[i] = m_max[i];
			}
			else
			{
				diagMin[i] = m_max[i];
				diagMax[i] = m_min[i];
			}
		}

		float test = plane.test(diagMin);
		if (test > 0.0f)
			return test;

		test = plane.test(diagMax);
		if (test >= 0.0f)
			return 0.0f;
		else
			return test;
	}
}



















