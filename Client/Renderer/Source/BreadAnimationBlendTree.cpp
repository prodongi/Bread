
#include "BreadAnimationBlendTree.h"
#include "BreadAnimationControlBox.h"
#include "BreadAnimationMotion.h"
#include "BreadAnimationBlendTreeLastResultNode.h"
#include "BreadAnimationBlendTreeChildNode.h"
#include "BreadAnimationBlendTreePose.h"

namespace Bread
{
	/*
	*/
	cAnimationBlendTree::cAnimationBlendTree() : m_root(NULL)
	{
	}
	/*
	*/
	cAnimationBlendTree::~cAnimationBlendTree()
	{
	}
	/*
	*/
	void cAnimationBlendTree::initialize(int jointNum, int poseMaxDepth)
	{
		m_poseMaxDepth = poseMaxDepth;
		m_poseEraseBuffer = new sAnimationBlendTreePoseEraseBuffer;
		m_result = new cAnimationBlendTreeLastResultNode(jointNum);
		m_root = new cAnimationBlendTreePose;
		m_root->setResultNode(m_result);
	}
	/*
	*/
	void cAnimationBlendTree::finalize()
	{
		safeDelete(m_poseEraseBuffer);
		safeDelete(m_result);
		safeDelete(m_root);
	}
	/*
	*/
	void cAnimationBlendTree::update(float elapsedtime)
	{
		if (m_root)
			m_root->update(elapsedtime);
	}
	/*
	*/
	void cAnimationBlendTree::blend(cAnimationController const* controller)
	{
		if (m_root)
		{
			m_root->blend(controller, m_poseEraseBuffer);
			poseEraseBuffer();
		}
	}
	/*
	*/
	void cAnimationBlendTree::poseEraseBuffer()
	{
		std::vector<cAnimationBlendTreePose*>::iterator it = m_poseEraseBuffer->m_list.begin();
		for (; it != m_poseEraseBuffer->m_list.end(); ++it)
		{
			erasePose(*it);
		}
		m_poseEraseBuffer->clear();
	}
	/*
	*/
	void cAnimationBlendTree::erasePose(cAnimationBlendTreePose* pose)
	{
		cAnimationBlendTreePose* leftPose = pose->getLeftPose();
		cAnimationBlendTreePose* rightPose = pose->getRightPose();

		if (m_root == pose)
		{
			if (isMainPose(pose))
			{
				pose->deleteStartNode();
			}
			else
			{
				if (rightPose)
				{
					rightPose->setLeftPose(NULL);
				}
				m_root = rightPose;
				safeDelete(pose);
			}
		}
		else
		{
			if (!isMainPose(pose))
			{
				if (leftPose)
					leftPose->setRightPose(rightPose);
				if (rightPose)
					rightPose->setLeftPose(leftPose);
				safeDelete(pose);
			}
		}
	}
	/*
	*/
	void cAnimationBlendTree::setMotion(cAnimationMotion* motion, int jointNum)
	{
		add(motion, jointNum, true);
	}
	/*
	*/
	void cAnimationBlendTree::addMotion(cAnimationMotion *motion, int jointNum)
	{
		add(motion, jointNum, false);
	}
	/*
	*/
	void cAnimationBlendTree::add(cAnimationMotion *motion, int jointNum, bool mainNode)
	{
		cAnimationBlendTreeNode* node = new cAnimationBlendTreeChildNode(jointNum);

		cAnimationControlBox controlBox;
		controlBox.setMotion(motion);
		node->setControlBox(controlBox);

		if (mainNode)
			m_mainNode = node;

		checkPoseMaxDepth();

		m_root->add(node, jointNum);
	}
	/*
	*/
	sAnimationResult* cAnimationBlendTree::getResult()
	{
		return m_result->getResult();
	}
	/*
	*/
	void cAnimationBlendTree::checkPoseMaxDepth()
	{
		int poseDepth = m_root->getDepth();
		if (poseDepth < m_poseMaxDepth)
			return ;

		cAnimationBlendTreePose* rightPose = m_root->getRightPose();

		if (isMainPose(m_root))
		{
			if (rightPose)
			{
				cAnimationBlendTreePose* rightPose2 = rightPose->getRightPose();
				m_root->setRightPose(rightPose2);
				safeDelete(rightPose);
			}
		}
		else
		{
			cAnimationBlendTreePose* oldRoot = m_root;
			m_root = rightPose;
			safeDelete(oldRoot);
		}
	}
	/*
	*/
	bool cAnimationBlendTree::isMainPose(cAnimationBlendTreePose const* pose) const
	{
		return pose->getEndNode() == m_mainNode;
	}
}