
#include "BreadAnimationBlendTypeNormal.h"
#include "BreadAnimationBlendTreeNode.h"
#include "BreadAnimationBlendTreeChildNode.h"
#include "BreadAnimationController.h"

namespace Bread
{
	/*
	*/
	void cAnimationBlendTypeNormal::blendResult(float blendRate, 
												cAnimationBlendTreeNode* startNode, 
												cAnimationBlendTreeNode* endNode, 
												cAnimationBlendTreeNode* resultNode, 
												cAnimationController const* controller)
	{
		if (!resultNode)
			return ;

		resultNode->slerpWorldTRS(blendRate, startNode, endNode, controller);
	}
}