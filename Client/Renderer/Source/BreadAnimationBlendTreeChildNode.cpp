
#include "BreadAnimationBlendTreeChildNode.h"
#include "BreadAnimationController.h"
#include "BreadAnimationBlendResult.h"
#include "BreadAnimationJoint.h"

namespace Bread
{
	implementNormalPool(cAnimationBlendTreeChildNode);

	/*
	*/
	cAnimationBlendTreeChildNode::cAnimationBlendTreeChildNode()
	{
	}
	/*
	*/
	cAnimationBlendTreeChildNode::cAnimationBlendTreeChildNode(int jointNum)
	{
		createResult(jointNum);
	}
	/*
	*/
	cAnimationBlendTreeChildNode::~cAnimationBlendTreeChildNode()
	{
	}
	/*
	*/
	void cAnimationBlendTreeChildNode::createResult(int jointNum)
	{
		assert(!m_result && _T("cAnimationBlendTreeNode::createResult"));

		m_jointNum = jointNum;
		m_result = new sAnimationInterResult;
		m_result->createJoint(jointNum);
	}
	/*
	*/
	void cAnimationBlendTreeChildNode::setControlBox(cAnimationControlBox const& controlBox)
	{
		m_controlBox = controlBox;
	}
	/*
	*/
	void cAnimationBlendTreeChildNode::blend(cAnimationBlendTreeNode* /*nextNode*/, cAnimationBlendTreeNode* /*resultNode*/)
	{
	}
	/*
	*/
	void cAnimationBlendTreeChildNode::update(float elapsedtime)
	{
		m_controlBox.update(elapsedtime);
	}
	/*
	*/
	void cAnimationBlendTreeChildNode::blend(cAnimationController const* controller)
	{
		if (!controller)
			return ;
		if (!m_controlBox.isMotion())
			return ;
		if (!m_result)
			return ;

		sAnimationFrame const* curFrame = m_controlBox.getCurFrame();

		if (m_controlBox.getCurFrameNum() + 1 >= m_controlBox.getFrameNum())
		{
			blendFrame(0.0f, curFrame, curFrame, controller);
		}
		else
		{
			float frameBlendRate = m_controlBox.getFrameBlendRate();

			sAnimationFrame const* nextFrame = m_controlBox.getNextFrame();
			blendFrame(frameBlendRate, curFrame, nextFrame, controller);
		}
	}
	/*
	*/
	void cAnimationBlendTreeChildNode::blendFrame(float blendRate, 
												  sAnimationFrame const* frame1, 
												  sAnimationFrame const* frame2, 
												  cAnimationController const* controller)
	{
		sVector3 pos;
		sQuaternion quat;

		for (int i = 0; i < m_jointNum; ++i)
		{
			int boneIndex = controller->getJointRefBoneIndex(i);

			if (0 <= boneIndex)
			{
				sJoint* joint1 = frame1->m_joint + boneIndex;
				sJoint* joint2 = frame2->m_joint + boneIndex;
				
				lerpVec3(joint1->m_pos, joint2->m_pos, blendRate, pos);
				quat.slerp(joint1->m_quat, joint2->m_quat, blendRate);

				sJointResult* jointResult = m_result->getJointResult(boneIndex);
				jointResult->set(pos, quat);
			}
		}
	}
}