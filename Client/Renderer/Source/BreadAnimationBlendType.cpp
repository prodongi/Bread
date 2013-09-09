
#include "BreadAnimationBlendType.h"
#include "BreadAnimationBlendTypeNormal.h"
#include "BreadAnimationBlendTreeNode.h"
#include "BreadAnimationBlendResult.h"
#include "BreadAnimationController.h"

namespace Bread
{
	/*
	*/
	cAnimationBlendType* cAnimationBlendTypeCreator::create(int type)
	{
		switch (type)
		{
		case BLEND_NORMAL:	return new cAnimationBlendTypeNormal;
		default:			return new cAnimationBlendTypeNormal;
		}
	}

	/*
	*/
	void cAnimationBlendType::setBlendTime(float t)
	{
		m_blendTime = t;
		m_elapsedtime = 0.0f;
	}
	/*
	*/
	float cAnimationBlendType::getBlendRate() const
	{
		float rate = min(1.0f, m_elapsedtime / m_blendTime);
		return rate;
	}
	/*
	*/
	void cAnimationBlendType::blendNode(cAnimationBlendTreeNode *node, cAnimationController const* controller)
	{
		node->blend(controller);
	}
	/*
	*/
	void cAnimationBlendType::update(float elapsedtime, cAnimationBlendTreeNode* startNode, cAnimationBlendTreeNode* endNode)
	{
		m_elapsedtime += elapsedtime;

		if (startNode) startNode->update(elapsedtime);
		if (endNode) endNode->update(elapsedtime);
	}
	/*
	*/
	bool cAnimationBlendType::blend(cAnimationBlendTreeNode* startNode, 
									cAnimationBlendTreeNode* endNode, 
									cAnimationBlendTreeNode* resultNode, 
									cAnimationController const* controller)
	{
		if (startNode)	blendNode(startNode, controller);
		if (endNode)	blendNode(endNode, controller);

		float  blendRate = getBlendRate();
		blendResult(blendRate, startNode, endNode, resultNode, controller);

		if (1.0f <= blendRate)
			return true;
		return false;
	}
}