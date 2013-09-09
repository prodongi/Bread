
#include "BreadDeclaration.h"
#include "BreadAnimationBlendTreePose.h"
#include "BreadAnimationBlendTreeNode.h"
#include "BreadAnimationBlendTreeInterResultNode.h"
#include "BreadAnimationBlendTreeChildNode.h"
#include "BreadAnimationBlendType.h"
#include "BreadAnimationController.h"
#include "BreadAnimationBlendResult.h"

namespace Bread
{
	/*
	*/
	cAnimationBlendTreePose::cAnimationBlendTreePose() : m_startNode(NULL),
														 m_endNode(NULL),
														 m_resultNode(NULL),
														 m_leftPose(NULL),
														 m_rightPose(NULL),
														 m_type(NULL)
	{
	}
	/*
	*/
	cAnimationBlendTreePose::~cAnimationBlendTreePose()
	{
		safeDelete(m_startNode);
		safeDelete(m_endNode);
		safeDelete(m_type);
	}
	/*
	*/
	void cAnimationBlendTreePose::setType(int type, float blendTime)
	{
		safeDelete(m_type);
		m_type = cAnimationBlendTypeCreator::create(type);
		m_type->setBlendTime(blendTime);
	}
	/*
	*/
	cAnimationBlendTreePose* cAnimationBlendTreePose::getLastPose()
	{
		if (m_rightPose)
			return m_rightPose->getLastPose();
		return this;
	}
	/*
	*/
	bool cAnimationBlendTreePose::add(cAnimationBlendTreeNode* node, int jointNum, int blendType, float blendTime)
	{
		cAnimationBlendTreePose* lastPose = getLastPose();

		if (NULL == lastPose->getStartNode() && NULL == lastPose->getEndNode())
		{
			lastPose->setEndNode(node);
			lastPose->setType(cAnimationBlendTypeCreator::BLEND_NORMAL, blendTime);
		}
		else if (NULL == lastPose->getStartNode())
		{
			lastPose->swapNode();
			lastPose->setEndNode(node);
			lastPose->setType(blendType, blendTime);
		}
		else if (NULL == lastPose->getEndNode())
		{
			lastPose->setEndNode(node);
			lastPose->setType(blendType, blendTime);
		}
		else
		{
			cAnimationBlendTreeNode* oldResult = lastPose->m_resultNode;

			cAnimationBlendTreePose* newPose = new cAnimationBlendTreePose;
			newPose->setStartNode(new cAnimationBlendTreeInterResultNode(jointNum));
			newPose->setEndNode(node);
			newPose->setResultNode(oldResult);
			newPose->setType(blendType, blendTime);

			lastPose->setResultNode(newPose->getStartNode());

			lastPose->setRightPose(newPose);
			newPose->setLeftPose(lastPose);
		}

		return true;
	}
	/*
	*/
	void cAnimationBlendTreePose::update(float elapsedTime)
	{
		if (m_type)
		{
			m_type->update(elapsedTime, m_startNode, m_endNode);
		}
	}
	/*
	*/
	void cAnimationBlendTreePose::blend(cAnimationController const* controller, sAnimationBlendTreePoseEraseBuffer* eraseBuffer)
	{
		if (m_type)
		{
			bool end = m_type->blend(m_startNode, m_endNode, m_resultNode, controller);
			if (end)
			{
				eraseBuffer->add(this);
			}
		}

		if (m_rightPose)
			m_rightPose->blend(controller, eraseBuffer);
	}
	/*
	*/
	void cAnimationBlendTreePose::swapNode()
	{
		std::swap(m_startNode, m_endNode);
	}
	/*
	*/
	void cAnimationBlendTreePose::deleteStartNode()
	{
		safeDelete(m_startNode);
	}
	/*
	*/
	int cAnimationBlendTreePose::getDepth()
	{
		int depth = 1;
		if (m_rightPose)
			depth += m_rightPose->getDepth();
		return depth;
	}
}