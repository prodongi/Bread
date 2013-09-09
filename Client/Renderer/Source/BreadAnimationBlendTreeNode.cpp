
#include "BreadAnimationBlendTreeNode.h"
#include "BreadAnimationBlendResult.h"

namespace Bread
{
	/*
	*/
	cAnimationBlendTreeNode::cAnimationBlendTreeNode() : m_result(NULL), m_jointNum(0)
	{
	}
	/*
	*/
	cAnimationBlendTreeNode::~cAnimationBlendTreeNode()
	{
		safeDelete(m_result);
	}
	/*
	*/
	void cAnimationBlendTreeNode::copyWorldTRS(cAnimationBlendTreeNode const* node, cAnimationController const* controller)
	{
		sAnimationResult* result = NULL;
		if (node)	result = node->getResult();

		m_result->copyWorldTRS(result, m_jointNum, controller);
	}
	/*
	*/
	void cAnimationBlendTreeNode::slerpWorldTRS(float blendRate, 
												cAnimationBlendTreeNode const* startNode, 
												cAnimationBlendTreeNode const* endNode, 
												cAnimationController const* controller)
	{
		sAnimationResult* startResult = NULL;
		sAnimationResult* endResult = NULL;

		if (startNode)	startResult = startNode->getResult();
		if (endNode)	endResult = endNode->getResult();

		if (startResult)
			m_result->slerpWorldTRS(blendRate, startResult, endResult, m_jointNum, controller);
		else
			m_result->copyWorldTRS(endResult, m_jointNum, controller);
	}
}