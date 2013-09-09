
#ifndef _BreadIBInfo9_h_
#define _BreadIBInfo9_h_

#include "BreadClientDefinition.h"
#include "BreadD3DHeader.h"
#include "BreadIndexBuffer.h"

namespace Bread
{
#pragma pack(1)

	/*
	*/
	struct sIBInfo
	{
		sIBInfo() : m_ibSize(0), m_faceNum(0)
		{
			m_usage = D3DUSAGE_WRITEONLY;
			m_pool = D3DPOOL_MANAGED;
			m_lock = 0;
		}
		bool lock(void** data);
		bool unlock();

		BUID m_buid;
		uint m_ibSize;
		uint m_usage;
		uint m_lock;
		uint m_faceNum;
		D3DPOOL m_pool;
		sIndexBuffer m_ib;
	};
	/*
	*/
	struct sIBCacheInfo
	{
		sIBCacheInfo() : m_buffer(NULL), m_bufferSize(0), m_faceNum(0)
		{
			m_usage = D3DUSAGE_WRITEONLY;
			m_pool = D3DPOOL_MANAGED;
			m_lock = 0;
		}
		~sIBCacheInfo();
		bool copy(sIBInfo* copyData, uint shareType);

		BUID m_buid;
		uint m_bufferSize;
		uint m_usage;
		uint m_lock;
		uint m_faceNum;
		D3DPOOL m_pool;
		char* m_buffer;
	};

#pragma pack()
}

#endif