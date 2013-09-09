
#include "BreadAnimationControlBox.h"
#include "BreadAnimationMotion.h"
#include "BreadAnimationJoint.h"

namespace Bread
{
	/*
	*/
	cAnimationControlBox::cAnimationControlBox() : m_curFrameNum(0), m_curMotionTime(0.0f), m_curMotion(NULL) 
	{
		m_tickPerFrame = 0.03f;
	}
	/*
	*/
	void cAnimationControlBox::setMotion(cAnimationMotion* motion)
	{
		m_curMotion = motion;
		m_curMotionTime = 0.0f;
		m_curFrameNum = 0;
	}
	/*
	*/
	void cAnimationControlBox::update(float elapsedtime)
	{
		if (!m_curMotion)
			return ;

		m_curMotionTime += elapsedtime;
		m_curFrameNum = (int)(m_curMotionTime/m_tickPerFrame);

		if (m_curFrameNum >= m_curMotion->getFrameNum())
		{
			m_curFrameNum = 0;
			m_curMotionTime = 0.0f;
		}
	}
	/*
	*/
	sAnimationFrame const* cAnimationControlBox::getCurFrame() const
	{
		if (!m_curMotion)
			return NULL;

		return m_curMotion->getFrame(m_curFrameNum);
	}
	/*
	*/
	sAnimationFrame const* cAnimationControlBox::getNextFrame() const
	{
		if (!m_curMotion)
			return NULL;

		return m_curMotion->getFrame(m_curFrameNum+1);
	}
	/*
	*/
	int cAnimationControlBox::getFrameNum() const
	{
		if (!m_curMotion)
			return 0;

		return m_curMotion->getFrameNum();
	}
	/*
	*/
	float cAnimationControlBox::getFrameBlendRate() const
	{
		float curFrameTime = (float)m_curFrameNum * m_tickPerFrame;
		float offsetTime = m_curMotionTime - curFrameTime;
		float blendRate = offsetTime/m_tickPerFrame;

		return blendRate;
	}
	/*
	*/
	void cAnimationControlBox::copy(cAnimationControlBox const* srcData)
	{
		m_tickPerFrame = srcData->m_tickPerFrame;
		m_curFrameNum = srcData->m_curFrameNum;
		m_curMotionTime = srcData->m_curMotionTime;
	}

}