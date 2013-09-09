
#ifndef _BreadAnimationBlendTreePose_h_
#define _BreadAnimationBlendTreePose_h_

namespace Bread
{
	class cAnimationBlendTreeNode;
	class cAnimationBlendType;
	class cAnimationController;
	class cAnimationControlBox;
	class cAnimationBlendTreePose;
	struct sAnimationResult;
	
	/*
	*/
	struct sAnimationBlendTreePoseEraseBuffer
	{
		~sAnimationBlendTreePoseEraseBuffer()	{	clear();		}
		void clear()							{	m_list.clear();	}
		void add(cAnimationBlendTreePose* pose)	{	m_list.push_back(pose);	}
		std::vector<cAnimationBlendTreePose*> m_list;
	};

	/*
		@date 2011.08.24
		@auth prodongi
		@desc 
		@todo 
		*/
	class cAnimationBlendTreePose
	{
	public:
		cAnimationBlendTreePose();
		~cAnimationBlendTreePose();

		void setType(int type, float blendTime);
		bool add(cAnimationBlendTreeNode* node, int jointNum, int blendType = 0, float blendTime = 0.4f);
		void update(float elapsedTime);
		void blend(cAnimationController const* controller, sAnimationBlendTreePoseEraseBuffer* eraseBuffer);
		void swapNode();
		void deleteStartNode();
		int getDepth();

		propertyConstFunc(cAnimationBlendTreeNode*, StartNode, m_startNode);
		propertyConstFunc(cAnimationBlendTreeNode*, EndNode, m_endNode);
		propertyConstFunc(cAnimationBlendTreeNode*, ResultNode, m_resultNode);
		propertyConstFunc(cAnimationBlendTreePose*, RightPose, m_rightPose);
		propertyConstFunc(cAnimationBlendTreePose*, LeftPose, m_leftPose);

	private:
		cAnimationBlendTreePose* getLastPose();

	private:
		cAnimationBlendType* m_type;

		cAnimationBlendTreeNode* m_startNode;
		cAnimationBlendTreeNode* m_endNode;
		cAnimationBlendTreeNode* m_resultNode;

		cAnimationBlendTreePose* m_rightPose;
		cAnimationBlendTreePose* m_leftPose;
	};
}



#endif