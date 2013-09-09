
#include "BreadAnimationEvent.h"

namespace Bread
{
	/*
	*/
	cAnimationEvent::cAnimationEvent()
	{
	}
	/*
	*/
	cAnimationEvent::~cAnimationEvent()
	{
		finalize();
	}
	/*
	*/
	void cAnimationEvent::initialize(int motionNum)
	{
		m_motion.reserve(motionNum);
	}
	/*
	*/
	void cAnimationEvent::finalize()
	{
		m_motion.clear();
	}
	/*
	*/
	void cAnimationEvent::copy(cAnimationEvent const* /*srcData*/)
	{
	}
	/*
	*/
	void cAnimationEvent::add(int /*motion*/, char const* /*type*/)
	{
	}
}