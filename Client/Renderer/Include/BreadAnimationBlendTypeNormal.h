
#ifndef _BreadAnimationBlendTypeNormal_h_
#define _BreadAnimationBlendTypeNormal_h_

#include "BreadAnimationBlendType.h"

namespace Bread
{
	class cAnimationBlendTreeNode;
	class cAnimationController;

	/*
		@date 2011.07.11
		@auth prodongi
		@desc n1 : cur frame node,
			  n2 : next frame node
		@todo 
	*/
	class cAnimationBlendTypeNormal : public cAnimationBlendType
	{
	protected:
		virtual void blendResult(float blendRate, cAnimationBlendTreeNode* startNode, cAnimationBlendTreeNode* endNode, 
								 cAnimationBlendTreeNode* resultNode, cAnimationController const* controller);
	};
}



#endif