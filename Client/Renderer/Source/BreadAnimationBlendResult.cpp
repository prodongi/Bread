
#include "BreadAnimationBlendResult.h"
#include "BreadAnimationController.h"

namespace Bread
{
	implementCachePool(sJointInterResult, cCachePool::MAX_POOL_NUM)
	implementCachePool(sJointLastResult, cCachePool::MAX_POOL_NUM)
	implementNormalPool(sAnimationInterResult)
	implementNormalPool(sAnimationLastResult)

	/*
	*/
	sJointResult* sAnimationInterResult::getJointResult(int index) const
	{
		return m_joint + index;
	}
	/*
	*/
	void sAnimationInterResult::createJoint(int jointNum)
	{
		m_joint = new sJointInterResult[jointNum];
	}
	/*
	*/
	void sAnimationInterResult::copyWorldTRS(sAnimationResult const* result, int jointNum, cAnimationController const* /*controller*/)
	{
		for (int i = 0; i < jointNum; ++i)
		{
			m_joint[i].copy(result->getJointResult(i));
		}
	}
	/*
	*/
	void sAnimationInterResult::slerpWorldTRS(float blendRate, 
											 sAnimationResult const* startResult, 
											 sAnimationResult const* endResult, 
											 int jointNum, 
											 cAnimationController const* /*controller*/)
	{
		for (int i = 0; i < jointNum; ++i)
		{
			m_joint[i].slerp(blendRate, startResult->getJointResult(i), endResult->getJointResult(i));
		}
	}

	/*
	*/
	sJointResult* sAnimationLastResult::getJointResult(int index) const
	{
		return m_joint + index;
	}
	/*
	*/
	void sAnimationLastResult::createJoint(int jointNum)
	{
		m_joint = new sJointLastResult[jointNum];
	}
	/*
	*/
	void sAnimationLastResult::copyWorldTRS(sAnimationResult const* result, int jointNum, cAnimationController const* controller)
	{
		sVector3 pos;
		sQuaternion quat;
		for (int i = 0; i < jointNum; ++i)
		{
			sJointResult* jointResult = result->getJointResult(i);
			jointResult->getValue(pos, quat);
			m_joint[i].transformWorldTRS(pos, quat, controller->getJointRefMatSkin(i));
		}
	}
	/*
	*/
	void sAnimationLastResult::slerpWorldTRS(float blendRate, 
											 sAnimationResult const* startResult, 
											 sAnimationResult const* endResult, 
											 int jointNum, 
											 cAnimationController const* controller)
	{
		sJointInterResult interJoint;
		for (int i = 0; i < jointNum; ++i)
		{
			interJoint.slerp(blendRate, startResult->getJointResult(i), endResult->getJointResult(i));
			m_joint[i].transformWorldTRS(interJoint.m_pos, interJoint.m_quat, controller->getJointRefMatSkin(i));
		}
	}
}