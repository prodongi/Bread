
#include "BreadAnimationBlendTreeLastResultNode.h"
#include "BreadAnimationBlendResult.h"

namespace Bread
{
	implementNormalPool(cAnimationBlendTreeLastResultNode);

	/*
	*/
	cAnimationBlendTreeLastResultNode::cAnimationBlendTreeLastResultNode()
	{
	}
	/*
	*/
	cAnimationBlendTreeLastResultNode::cAnimationBlendTreeLastResultNode(int jointNum)
	{
		createResult(jointNum);
	}
	/*
	*/
	cAnimationBlendTreeLastResultNode::~cAnimationBlendTreeLastResultNode()
	{
	}
	/*
	*/
	void cAnimationBlendTreeLastResultNode::createResult(int jointNum)
	{
		assert(!m_result && _T("cAnimationBlendTreeLastResultNode::createResult"));

		m_jointNum = jointNum;
		m_result = new sAnimationLastResult;
		m_result->createJoint(jointNum);
	}
}