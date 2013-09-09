
#ifndef _BreadAnimationControlBox_h_
#define _BreadAnimationControlBox_h_

#include "BreadDeclaration.h"

namespace Bread
{
	class cAnimationMotion;
	struct sAnimationFrame;

	/*
		@date 2011.08.30
		@auth prodongi
		@desc 
		@todo 
	*/
	class cAnimationControlBox
	{
	public:
		cAnimationControlBox();

		bool isMotion() const	{	return m_curMotion != NULL;	}
		int getCurFrameNum() const	{	return m_curFrameNum;	}
		void setMotion(cAnimationMotion* motion);
		void update(float elapsedtime);
		sAnimationFrame const* getCurFrame() const;
		sAnimationFrame const* getNextFrame() const;
		int getFrameNum() const;
		float getFrameBlendRate() const;
		void copy(cAnimationControlBox const* srcData);

	private:
		float m_tickPerFrame;
		int m_curFrameNum;
		float m_curMotionTime;			/// @brief 현재 모션의 경과된 시간
		cAnimationMotion* m_curMotion;
	};
}



#endif