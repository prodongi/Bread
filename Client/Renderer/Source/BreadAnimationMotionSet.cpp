
#include "BreadAnimationMotionSet.h"

namespace Bread
{
	/*
	*/
	struct sAnimationSetCopy
	{
		sAnimationSetCopy(cAnimationMotionSet* motionSet, int jointNum)
		{
			m_motinSet = motionSet;
			m_jointNum = jointNum;
		}
		void operator()(cAnimationMotion const* srcData)
		{
			cAnimationMotion* motion = new cAnimationMotion;
			motion->copy(srcData, m_jointNum);
			m_motinSet->insert(srcData->getBuid(), &motion);
		}
		cAnimationMotionSet* m_motinSet;
		int m_jointNum;
	};

	/*
	*/
	cAnimationMotionSet::cAnimationMotionSet()
	{
	}
	/*
	*/
	cAnimationMotionSet::~cAnimationMotionSet()
	{
	}	
	/*
	*/
	void cAnimationMotionSet::copy(cAnimationMotionSet* srcData, int jointNum)
	{
		srcData->forEach(sAnimationSetCopy(this, jointNum));
	}
	/*
	*/
	cAnimationMotion* cAnimationMotionSet::getMotion(BUID const& motionBuid) const
	{
		cAnimationMotion* motion = get(motionBuid);
		if (!motion && 0 != getCount())
		{
			motion = getFirst();
		}
		return motion;
	}
}