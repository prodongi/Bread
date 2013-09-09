
#ifndef _BreadAnimationMotionManager_h_
#define _BreadAnimationMotionManager_h_

#include "BreadClientDefinition.h"
#include "BreadBucketHash.h"
#include "BreadBucketMap.h"
#include "BreadAnimationAction.h"

#define _getAnimationActionMgr()	Bread::cAnimationActionManager::getInstance()

namespace Bread
{
	#define ENTITYMOTION_BUCKET_SIZE	100

	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	class cAnimationActionManager : public cBucketHash<std_string, cAnimationAction*, ENTITYMOTION_BUCKET_SIZE>
	{
		declareInstance(cAnimationActionManager)
		declareClassCreater(cAnimationActionManager)

	public:
		virtual void initialize();
		virtual void finalize();
		BUID const& getBuid(uint id);

	protected:
		void insert(BUID const& buid, uint id, cAnimationAction* entityMotion);
		/// registAction()���� ȣ��, �ش� �Լ��� ������ �����ν� ���� Ÿ���� �׼��� ���� �� �� �ִ�.
		virtual cAnimationAction* createAction(int type);

	private:
		/// @brief lua���Ͽ��� action table�� �о�� m_idTable�� ����Ѵ�.
		/// ���ο� �׼��� �߰� �ϱ� ���ؼ��� createAction()�� ������ �ϸ� �ȴ�.
		void registAction();

	protected:
		cBucketMap<uint, BUID> m_idTable;

		static char const* ANIMATION_ACTION_LUA_TABLE_NAME;
	};
}



#endif