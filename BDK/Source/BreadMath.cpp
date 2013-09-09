
#include "BreadMath.h"
#include "BreadVector3.h"
#include "BreadRand.h"
#include "BreadInline.h"

namespace Bread
{
	float const cMath::EPSILON	= 1.0e-6f;
	float const cMath::PI		= 3.1415926535897932384626433832795f;
	float const cMath::HALFPI	= 1.5707963267948966192313216916398f;
	float const cMath::TWOPI	= 6.283185307179586476925286766558f;
	sRand* cMath::randGenerator = NULL;

	/*
	*/
	void cMath::clear()
	{
		safeDelete(randGenerator);
	}
	/*
	*/
	void cMath::initRand(int type)
	{
		::srand((unsigned int)::time(NULL));

		if (0 == type)		randGenerator = new sRandLinearCongruential;
		else if (1 == type)	randGenerator = new sRandMersenneTwister;
		else if (2 == type) randGenerator = new sRandWellRng512;
		else				randGenerator = new sRandLinearCongruential;

		randGenerator->initialize();
	}
	/*
	*/
	unsigned long cMath::rand()
	{
		assert(randGenerator);
		return randGenerator->rand();
	}
	/*
	*/
	unsigned long cMath::rand(unsigned long range)
	{
		if (0 == range)
		{
			assert(0 && "invalid random range");
			return 0;
		}

		assert(randGenerator);
		return randGenerator->rand(range);
	}
	/*
	*/
	float cMath::rand(float amount, float range)
	{
		if (cMath::isZero(range))
		{
			assert(0 && "invalid random range");
			return 0.0f;
		}

		if (!randGenerator)
		{
			assert(randGenerator);
			return 0.0f;
		}
		float r = (float)randGenerator->rand((unsigned long)range);
		return amount * r / range;
	}
	/*
	 *	
	 */
	bool cMath::isInFace(sVector3 const* face, sVector3 const& p)
	{
		return isInFace(face[0], face[1], face[2], p);
	}
	/*
	 *	
	 */
	bool cMath::isInFace(sVector3 const& face0, sVector3 const& face1, sVector3 const& face2, sVector3 const& p)
	{
		float sign1, sign2, sign3;
		static sVector3 vab, vbc, vca, vat, vbt, vct;

		subVec3(face1, face0, vab);
		subVec3(face2, face1, vbc);
		subVec3(face0, face2, vca);

		subVec3(p, face0, vat);
		subVec3(p, face1, vbt);
		subVec3(p, face2, vct);

		sign1 = cross2Vec3(vab, vat);
		sign2 = cross2Vec3(vbc, vbt);
		sign3 = cross2Vec3(vca, vct);

		if ((sign1 >= 0 && sign2 >= 0 && sign3 >= 0) || (sign1 < 0 && sign2 < 0 && sign3 < 0))
		{
			return true;
		}
		return false;
	}
	/*
	 *	
	 */
	void cMath::getHeightAtFace(sVector3 const& face0, sVector3 const& face1, sVector3 const& face2, sVector3& p)
	{
		static sVector3 vab, vbc, n;

		subVec3(face1, face0, vab);
		subVec3(face2, face1, vbc);
		vab.cross(vbc, n);
		n.normalize();

		float d = -(n.x*face1.x + n.y*face1.y + n.z*face1.z);
		p.y = -(n.x*p.x + n.z*p.z + d)/n.y;
	}
	/*
	*/
	unsigned int cMath::getLargestPow2(unsigned int num)
	{
		unsigned int p = 1;
		bool is = true;
		while (is)
		{
			if (p > num)
				return p;
			p = p << 1;
		}
		return 0;
	}
}