
#ifndef _BreadAnimationBlendTreeNode_h_
#define _BreadAnimationBlendTreeNode_h_

#include "BreadAnimationControlBox.h"
#include "BreadBoostNormalPool.h"

namespace Bread
{
	struct sAnimationResult;
	class cAnimationControlBox;
	class cAnimationController;

	/*
		@date 2011.08.30
		@auth prodongi
		@desc 
		@todo 
	*/
	class cAnimationBlendTreeNode
	{
	public:
		cAnimationBlendTreeNode();
		virtual ~cAnimationBlendTreeNode();

		virtual void setControlBox(cAnimationControlBox const& /*controlBox*/) {}
		virtual void blend(cAnimationBlendTreeNode* /*nextNode*/, cAnimationBlendTreeNode* /*resultNode*/) {}
		virtual void update(float /*elapsedtime*/) {}
		virtual void blend(cAnimationController const* /*controller*/) {}
		
		virtual int getJointNum() const								{	return m_jointNum;	}
		virtual sAnimationResult* getResult() const					{	return m_result;	}
		virtual cAnimationControlBox const* getControlBox() const	{	return NULL;		}

		void copyWorldTRS(cAnimationBlendTreeNode const* result, cAnimationController const* controller);
		void slerpWorldTRS(float blendRate, cAnimationBlendTreeNode const* startNode, cAnimationBlendTreeNode const* endNode, cAnimationController const* controller);

	protected:		
		virtual void createResult(int /*jointNum*/) {}

	protected:
		sAnimationResult* m_result;
		int m_jointNum;
	};
}



#endif