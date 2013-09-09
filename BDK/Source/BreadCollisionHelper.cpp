
#include "BreadCollisionHelper.h"

namespace Bread
{
	/*
	*/
	cCollisionHelper::cCollisionHelper()
	{
	}
	/*
	*/
	cCollisionHelper::~cCollisionHelper()
	{
	}
	/* Ds = (P - Ps) * Normal
	*/
	float cCollisionHelper::perpendicularDistance(sVector3 const& p, sVector3 const& ps, sVector3 const& normal)
	{
		sVector3 pi;
		subVec3(p, ps, pi);
		return pi.dot(normal);
	}
	/* ret = P - Ds*N
	*/
	void cCollisionHelper::perpendicularPoint(sVector3 const& p, sVector3 const& ps, sVector3 const& normal, sVector3& ret)
	{
		float d = cCollisionHelper::perpendicularDistance(p, ps, normal);
		subMulVec3(p, normal, d, ret);
	}
	/* Pc = P + R(Pf - P), 
		R = Ds/((P - Pf)*N)
	*/
	void cCollisionHelper::intersectionPoint(sVector3 const& p, sVector3 const& pf, sVector3 const& ps, sVector3 const& normal, sVector3& ret)
	{
		float d1 = cCollisionHelper::perpendicularDistance(p, ps, normal);
		float d2 = cCollisionHelper::perpendicularDistance(p, pf, normal);
		float r = d1 * cMath::inv(d2, 1.0f);

		if (cMath::isZero(r))
		{
			ret = p;
		}
		else if (cMath::equal(r, 1.0f))
		{
			ret = pf;
		}
		else
		{
			// ret = p + r * (pf - p);
			sVector3 temp;
			subVec3(pf, p, temp);
			addMulVec3(p, temp, r, ret);
		}
	}
	/*
	*/
	float cCollisionHelper::intersectionDistance(sVector3 const& p, sVector3 const& pf, sVector3 const& ps, sVector3 const& normal)
	{
		sVector3 ret;
		intersectionPoint(p, pf, ps, normal, ret);
		return p.length(ret);
	}
	/*
	*/
	void cCollisionHelper::reflectionVector(sVector3 const& p, sVector3 const& pf, sVector3 const& ps, sVector3 const& normal, sVector3& refPoint, sVector3& refVector)
	{
		// 반사 포인터를 구한다.
		float len = perpendicularDistance(pf, ps, normal);
		len *= 2.0f;
		sVector3 _n;
		mulVec3(normal, len, _n);
		addVec3(pf, _n, refPoint);

		// 교차점을 구한다.
		sVector3 pc;
		intersectionPoint(p, pf, ps, normal, pc);

		// 교차점에서의 반사 포인터로의 방향 벡터를 구한다.
		sVector3 prc;
		subVec3(refPoint, pc, prc);
		prc.normalize();

		sVector3 pcf;
		subVec3(p, pc, pcf);

		mulVec3(prc, pcf.length(), refVector);
	}
	/*
	*/
	void cCollisionHelper::withDrawVector(sVector3 const& p, sVector3 const& pf, sVector3 const& ps, sVector3 const& normal, sVector3& ret)
	{
		sVector3 pc;
		intersectionPoint(p, pf, ps, normal, pc);

		// ret = pc + (pc - pf) = 2pc - pf
		subVec3(2.0f * pc, pf, ret);
	}
}