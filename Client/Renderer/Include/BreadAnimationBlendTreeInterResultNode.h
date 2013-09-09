
#ifndef _BreadAnimationBlendTreeInterResultNode_h_
#define _BreadAnimationBlendTreeInterResultNode_h_

#include "BreadAnimationBlendTreeNode.h"
#include "BreadBoostNormalPool.h"

namespace Bread
{
	/*
		@date 2011.08.30
		@auth prodongi
		@desc 
		@todo 
	*/
	class cAnimationBlendTreeInterResultNode : public cAnimationBlendTreeNode
	{
		declareNormalPool(cAnimationBlendTreeInterResultNode);

	public:
		cAnimationBlendTreeInterResultNode();
		cAnimationBlendTreeInterResultNode(int jointNum);
		virtual ~cAnimationBlendTreeInterResultNode();

	protected:
		virtual void createResult(int jointNum);
	};

}



#endif