
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
		/// @param get()�Լ��� Ʋ�� ���� motionBuid�� ã�� �� ���� ��, �� ó�� ����� ������ �ش�
		cAnimationMotion* getMotion(BUID const& motionBuid) const;
	};
}



#endif