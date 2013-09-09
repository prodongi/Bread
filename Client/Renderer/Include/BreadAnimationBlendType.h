
#ifndef _BreadAnimationBlendType_h_
#define _BreadAnimationBlendType_h_

#include "BreadClientDefinition.h"

namespace Bread
{
	class cAnimationBlendTreeNode;
	class cAnimationController;
	class cAnimationBlendType;
	struct sAnimationFrame;
	struct sAnimationResult;

	/*
	*/
	class cAnimationBlendTypeCreator
	{
	public:
		enum {	BLEND_NORMAL };

	public:
		static cAnimationBlendType* create(int type);
	};

	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	/*
	*/
	class cAnimationBlendType
	{
	public:
		void setBlendTime(float t);
		virtual void update(float elapsedtime, cAnimationBlendTreeNode* startNode, cAnimationBlendTreeNode* endNode);
		virtual bool blend(cAnimationBlendTreeNode* startNode, cAnimationBlendTreeNode* endNode, cAnimationBlendTreeNode* resultNode, cAnimationController const* controller);

	protected:
		float getBlendRate() const;
		virtual void blendNode(cAnimationBlendTreeNode* /*node*/, cAnimationController const* /*controller*/);
		virtual void blendResult(float /*blendRate*/, cAnimationBlendTreeNode* /*startNode*/, cAnimationBlendTreeNode* /*endNode*/, 
								 cAnimationBlendTreeNode* /*resultNode*/, cAnimationController const* /*controller*/) {}

	protected:
		float m_blendTime;
		float m_elapsedtime;
	};
}



#endif