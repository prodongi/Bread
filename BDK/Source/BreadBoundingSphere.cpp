
#include "BreadBoundingSphere.h"
#include "BreadLine.h"

namespace Bread
{
	/**
	 *	
	 */
	void cBoundingSphere::set(const sVector3* vecList, unsigned int vertNum)
	{
		//
		sVector3 minVec(vecList[0]);
		sVector3 maxVec(vecList[0]);

		for (unsigned int i = 0; i < vertNum; ++i)
		{
			const sVector3* p = vecList + i;
			if (p->x < minVec.x)		minVec.x = p->x;
			else if (p->x > maxVec.x)	maxVec.x = p->x;

			if (p->y < minVec.y)		minVec.y = p->y;
			else if (p->y > maxVec.y)	maxVec.y = p->y;

			if (p->z < minVec.z)		minVec.z = p->z;
			else if (p->z > maxVec.z)	maxVec.z = p->z;
		}

		sVector3 temp;
		addVec3(minVec, maxVec, temp);
		mulVec3(temp, 0.5f, m_center);

		float maxDist = m_center.lengthSquare(vecList[0]);
		for (unsigned int i = 0; i < vertNum; ++i)
		{
			float dist = m_center.lengthSquare(vecList[i]);
			if (dist > maxDist)
				maxDist = dist;
		}
		m_radius = cMath::sqrtf(maxDist);
	}
	/**
	 *	
	 */
	bool cBoundingSphere::collision(const cBoundingSphere& sphere)
	{
		float centerLength = m_center.lengthSquare(sphere.m_center);
		float sumRadius = m_radius + sphere.m_radius;
		return centerLength <= sumRadius*sumRadius;
	}
}


















