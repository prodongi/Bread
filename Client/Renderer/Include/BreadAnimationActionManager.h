
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
		/// registAction()에서 호출, 해당 함수를 재정의 함으로써 많은 타입의 액션을 생성 할 수 있다.
		virtual cAnimationAction* createAction(int type);

	private:
		/// @brief lua파일에서 action table을 읽어와 m_idTable에 등록한다.
		/// 새로운 액션을 추가 하기 위해서는 createAction()을 재정의 하면 된다.
		void registAction();

	protected:
		cBucketMap<uint, BUID> m_idTable;

		static char const* ANIMATION_ACTION_LUA_TABLE_NAME;
	};
}



#endif