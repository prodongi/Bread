
#ifndef _BreadAnimationBlendTreeChildNode_h_
#define _BreadAnimationBlendTreeChildNode_h_

#include "BreadAnimationBlendTreeNode.h"
#include "BreadBoostNormalPool.h"

namespace Bread
{
	class cAnimationController;

	/*
		@date 2011.08.30
		@auth prodongi
		@desc 
		@todo 
	*/
	class cAnimationBlendTreeChildNode : public cAnimationBlendTreeNode
	{
		declareNormalPool(cAnimationBlendTreeChildNode);

	public:
		cAnimationBlendTreeChildNode();
		cAnimationBlendTreeChildNode(int jointNum);
		virtual ~cAnimationBlendTreeChildNode();

		virtual void setControlBox(cAnimationControlBox const& controlBox);
		virtual void blend(cAnimationBlendTreeNode* nextNode, cAnimationBlendTreeNode* resultNode);
		virtual void update(float elapsedtime);
		virtual void blend(cAnimationController const* controller);

		virtual cAnimationControlBox const* getControlBox() const	{	return &m_controlBox;	}

	protected:
		virtual void createResult(int jointNum);
		void blendFrame(float blendRate, sAnimationFrame const* frame1, sAnimationFrame const* frame2, cAnimationController const* controller);

	protected:
		cAnimationControlBox m_controlBox;
	};
}



#endif