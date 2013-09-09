
#include "BreadIBInfo.h"
#include "BreadStringHelper.h"
#include "BreadIBManager.h"
#include "BreadDebug.h"
#include "BreadInline.h"

namespace Bread
{
	/*
	*/
	bool sIBInfo::lock(void** data)
	{
		return m_ib.lock(0, m_ibSize, data, m_lock);
	}
	/*
	*/
	bool sIBInfo::unlock()
	{
		return m_ib.unlock();
	}
	/*
	*/
	sIBCacheInfo::~sIBCacheInfo()
	{
		safeDeleteArray(m_buffer);
	}
	/*
	*/
	bool sIBCacheInfo::copy(sIBInfo* copyData, uint shareType)
	{
		copyData->m_ibSize = m_bufferSize;
		copyData->m_usage = m_usage;
		copyData->m_lock = m_lock;
		copyData->m_faceNum = m_faceNum;
		copyData->m_pool = m_pool;
		if (ST_IB & shareType)
		{
			copyData->m_buid = m_buid;
		}
		else
		{
			copyData->m_buid = _getStringHelper()->make(_T("%s_%d"), m_buid.c_str(), _getIBMgr()->getCount());
		}

		if (!_getIBMgr()->create(copyData, m_buffer))
			return false;

		return true;
	}
}