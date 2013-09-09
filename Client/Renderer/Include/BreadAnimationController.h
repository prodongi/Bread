
#ifndef _BreadAnimationController_h_
#define _BreadAnimationController_h_

#include "BreadDeclaration.h"
#include "BreadVector3.h"
#include "BreadMatrix4.h"

namespace Bread
{
	struct sJointRef;
	struct sJointResult;
	struct sAnimationResult;
	struct sAnimationFrame;
	class cAnimationControlBox;
	class cRenderData;
	class cAnimationEvent;
	class cAnimationMotion;
	class cAnimationBlendTree;

	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	class cAnimationController
	{
	public:
		cAnimationController();
		~cAnimationController();
		void initialize(int jointNum, bool useGpu);
		void finalize();
		void update(float elapsedtime);
		void setMotion(BUID const& motionBuid, cAnimationMotion* motion, cAnimationEvent* event);
		void renderBone(sVector3 const& pos);
		void skinning(cRenderData* renderData);
		void copy(cAnimationController const* srcData);
		int getJointRefBoneIndex(int jointIndex) const;
		sMatrix4 const& getJointRefMatSkin(int jointIndex) const;

		propertyFunc(bool, Pause, m_pause)
		propertyFunc(bool, UseGpu, m_useGpu)
		int getJointNum() const					{	return m_jointNum;		}
		BUID const& getCurMotionBuid() const	{	return m_curMotionBuid;	}

	private:
		cAnimationControlBox* m_controlBox;
		BUID m_curMotionBuid;
		int	m_jointNum;			/// @brief joint °³¼ö		
		bool m_useGpu;
		bool m_pause;

		sJointRef* m_jointRef;
		cAnimationEvent* m_event;
		cAnimationMotion* m_curMotion;
		cAnimationBlendTree* m_blendTree;

		friend class cSerialize3D;
	};
}



#endif