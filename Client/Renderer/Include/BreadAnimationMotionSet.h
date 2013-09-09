
#ifndef _BreadAnimationMotionSet_h_
#define _BreadAnimationMotionSet_h_

#include "BreadBucketHash.h"
#include "BreadAnimationMotion.h"

namespace Bread
{
	#define ANIMATIONMOTIONSET_BUCKET_SIZE	10

	/*
	*/
	class cAnimationMotionSet : public cBucketHash<BUID, cAnimationMotion*, ANIMATIONMOTIONSET_BUCKET_SIZE>
	{
	public:
		cAnimationMotionSet();
		~cAnimationMotionSet();
		void copy(cAnimationMotionSet* srcData, int jointNum);
		/// @param get()함수와 틀린 점은 motionBuid를 찾지 못 했을 때, 맨 처음 모션을 리턴해 준다
		cAnimationMotion* getMotion(BUID const& motionBuid) const;
	};
}



#endif