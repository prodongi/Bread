
#include "BreadAnimationMotion.h"
#include "BreadAnimationJoint.h"

namespace Bread
{
	/*
	*/
	cAnimationMotion::cAnimationMotion() : m_frameNum(0), 
										   m_frame(NULL),
										   m_loop(false)
	{
	}
	/*
	*/
	cAnimationMotion::~cAnimationMotion()
	{
		safeDeleteArray(m_frame);
	}
	/*
	*/
	void cAnimationMotion::copy(cAnimationMotion const* srcData, int jointNum)
	{
		m_buid = srcData->m_buid;
		m_frameNum = srcData->m_frameNum;
		m_frame = new sAnimationFrame[m_frameNum];
		for (int i = 0; i < m_frameNum; ++i)
		{
			m_frame[i].copy(srcData->m_frame[i], jointNum);
		}
	}
	/*
	*/
	sAnimationFrame const* cAnimationMotion::getFrame(int frame) const
	{
		if (0 > frame)
		{
			assert(0 && _T("invalid frame num(0 > frame)"));
			return NULL;
		}
		if (m_frameNum <= frame)
		{
			assert(0 && _T("invalid frame num(m_frameNum <= frame)"));
			return NULL;
		}
		return m_frame + frame;
	}
}