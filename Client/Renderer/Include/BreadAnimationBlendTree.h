
#ifndef _BreadAnimationBlendTree_h_
#define _BreadAnimationBlendTree_h_

namespace Bread
{
	class cAnimationBlendTreePose;
	class cAnimationBlendTreeNode;
	class cAnimationControlBox;
	class cAnimationMotion;
	class cAnimationController;
	struct sAnimationResult;
	struct sAnimationBlendTreePoseEraseBuffer;

	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	class cAnimationBlendTree
	{
	private:
		enum {	DEFAULT_POSE_MAX_DEPTH = 5 };

	public:
		cAnimationBlendTree();
		~cAnimationBlendTree();

		void initialize(int jointNum, int poseMaxDepth = DEFAULT_POSE_MAX_DEPTH);
		void finalize();
		void update(float elapsedtime);
		void blend(cAnimationController const* controller);
		void setMotion(cAnimationMotion* motion, int jointNum);
		void addMotion(cAnimationMotion* motion, int jointNum);
		sAnimationResult* getResult();

	private:
		void add(cAnimationMotion *motion, int jointNum, bool mainNode);
		void poseEraseBuffer();
		void erasePose(cAnimationBlendTreePose* pose);
		void checkPoseMaxDepth();
		bool isMainPose(cAnimationBlendTreePose const* pose) const;

	private:
		cAnimationBlendTreePose* m_root;
		cAnimationBlendTreeNode* m_result;
		cAnimationBlendTreeNode* m_mainNode;
		sAnimationBlendTreePoseEraseBuffer* m_poseEraseBuffer;
		int m_poseMaxDepth;
	};
}



#endif