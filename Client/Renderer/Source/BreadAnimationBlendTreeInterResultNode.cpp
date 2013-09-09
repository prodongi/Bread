
#include "BreadAnimationBlendTreeInterResultNode.h"
#include "BreadAnimationBlendResult.h"

namespace Bread
{
	implementNormalPool(cAnimationBlendTreeInterResultNode);

	/*
	*/
	cAnimationBlendTreeInterResultNode::cAnimationBlendTreeInterResultNode()
	{
	}
	/*
	*/
	cAnimationBlendTreeInterResultNode::cAnimationBlendTreeInterResultNode(int jointNum)
	{
		createResult(jointNum);
	}
	/*
	*/
	cAnimationBlendTreeInterResultNode::~cAnimationBlendTreeInterResultNode()
	{
	}
	/*
	*/
	void cAnimationBlendTreeInterResultNode::createResult(int jointNum)
	{
		assert(!m_result && _T("cAnimationBlendTreeInterResultNode::createResult"));

		m_jointNum = jointNum;
		m_result = new sAnimationInterResult;
		m_result->createJoint(jointNum);
	}
}