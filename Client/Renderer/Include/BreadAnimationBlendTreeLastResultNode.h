
#ifndef _BreadAnimationBlendTreeLastResultNode_h_
#define _BreadAnimationBlendTreeLastResultNode_h_

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
	class cAnimationBlendTreeLastResultNode : public cAnimationBlendTreeNode
	{
		declareNormalPool(cAnimationBlendTreeLastResultNode);

	public:
		cAnimationBlendTreeLastResultNode();
		cAnimationBlendTreeLastResultNode(int jointNum);
		virtual ~cAnimationBlendTreeLastResultNode();

	protected:
		virtual void createResult(int jointNum);
	};

}



#endif