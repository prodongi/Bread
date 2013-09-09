
#include "BreadUUIDManager.h"

namespace Bread
{
	implementSingleton(cUUIDManager)

	static TCHAR* divisionToken = _T("_");

	/*
	*/
	cUUIDManager::cUUIDManager()
	{
		m_count = 0;
	}
	/*
	*/
	void cUUIDManager::incCount()
	{
		++m_count;
	}
	/*
	*/
	void cUUIDManager::make(uUUID& uuid)
	{
		incCount();
		uuid.m_key.m_id = m_count;
		uuid.m_key.m_tick = (uint)timeGetTime();
	}
	/*
	*/
	void cUUIDManager::encode(BUID& buid)
	{
		uUUID uuid;
		make(uuid);

		_stprintf_s(m_buffer, MAX_PATH, _T("%s%I64d"), divisionToken, uuid.m_number);
		buid += m_buffer;
	}
	/*
	*/
	void cUUIDManager::decode(BUID& uuidBuid)
	{
		size_t pos = uuidBuid.find_last_of(divisionToken);
		uuidBuid.erase(pos);
	}
}