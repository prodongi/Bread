
#ifndef _BreadLuaManager_h_
#define _BreadLuaManager_h_

#include "BreadClientDefinition.h"
#include "BreadLuaManagerBase.h"
#include "BreadLuaTable.h"

namespace Bread
{
	/*
		@date 2010.00.00
		@auth prodongi
		@desc init()가 호출 되면 init.lua파일을 로드한다.
		@todo 
	*/
	class cLuaManager : public cLuaManagerBase
	{
		declareClassCreater(cLuaManager)

	public:
		virtual void init();

	protected:
		/// @brief init()에서 호출, registerLuaFunc_xxx()를 호출
		virtual void registerGlueFuncs();
		/// @brief init()에서 호출
		virtual void registerPredefineMembers();
		/// @brief init()에서 호출, init.lua파일 로드
		virtual void loadLuaFiles();

	private:
		void registerPredefinePostEffect();
		void registerPredefineAnimaion();
	};
}



#endif