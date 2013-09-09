
#ifndef _BreadUUID_h_
#define _BreadUUID_h_

#include <windows.h>
#include <tchar.h>
#include "BreadSingleton.h"
#include "BreadDefinition.h"

namespace Bread
{
	struct sUUID
	{
		uint m_id;
		uint m_tick;
	};
	union uUUID
	{
		sUUID m_key;
		__int64 m_number;
	};
	/*
		@date 2011.05.29
		@auth prodongi
		@desc UUID를 생성하는 클래스
		@todo 
	*/
	class cUUIDManager
	{
		declareSingleton(cUUIDManager)

	public:
		~cUUIDManager() {}
		void encode(BUID& buid);		/// @brief buid -> uuid
		void decode(BUID& uuidBuid);	/// @brief uuid -> buid

	private:		
		void make(uUUID& uuid);
		void incCount();

	private:
		uint m_count;
		TCHAR m_buffer[MAX_PATH];
	};
}



#endif