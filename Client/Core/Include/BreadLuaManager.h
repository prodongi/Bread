
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
		@desc init()�� ȣ�� �Ǹ� init.lua������ �ε��Ѵ�.
		@todo 
	*/
	class cLuaManager : public cLuaManagerBase
	{
		declareClassCreater(cLuaManager)

	public:
		virtual void init();

	protected:
		/// @brief init()���� ȣ��, registerLuaFunc_xxx()�� ȣ��
		virtual void registerGlueFuncs();
		/// @brief init()���� ȣ��
		virtual void registerPredefineMembers();
		/// @brief init()���� ȣ��, init.lua���� �ε�
		virtual void loadLuaFiles();

	private:
		void registerPredefinePostEffect();
		void registerPredefineAnimaion();
	};
}



#endif