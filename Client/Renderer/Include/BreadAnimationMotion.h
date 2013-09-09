
#ifndef _BreadAnimationMotion_h_
#define _BreadAnimationMotion_h_

#include "BreadDeclaration.h"

namespace Bread
{
	struct sAnimationFrame;

	/*
		@date 2011.07.21
		@auth prodongi
		@desc 
		@todo 
	*/
	class cAnimationMotion
	{
	public:
		cAnimationMotion();
		~cAnimationMotion();

		int getFrameNum() const	{	return m_frameNum;	}
		sAnimationFrame const* getFrame(int frame) const;
		void copy(cAnimationMotion const* srcData, int jointNum);
		propertyConstFunc(bool, Loop, m_loop)
		propertyConstFunc(BUID, Buid, m_buid)

	private:
		BUID m_buid;
		int m_frameNum;
		sAnimationFrame* m_frame;
		bool m_loop;

		friend class cSerialize3D;
	};
}



#endif