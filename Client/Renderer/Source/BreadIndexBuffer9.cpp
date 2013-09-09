
#include "BreadIndexBuffer9.h"
#include "BreadInline.h"
#include "BreadDebug.h"
#include "BreadD3DSystem.h"

namespace Bread
{
	/*
	*/
	sIndexBuffer::sIndexBuffer() : m_stream(NULL)
	{
	}
	/*
	*/
	void sIndexBuffer::release()
	{
		safeRelease(m_stream);
	}
	/*
	*/
	bool sIndexBuffer::lock(uint offset, uint size, void** data, DWORD _lock)
	{
		if (!checkHResultReturn(m_stream->Lock(offset, size, data, _lock)))
			return false;
		return true;
	}
	/*
	*/
	bool sIndexBuffer::unlock()
	{
		if (!checkHResultReturn(m_stream->Unlock()))
			return false;
		return true;
	}
	/*
	*/
	bool sIndexBuffer::createStream(uint size, uint usage, D3DPOOL pool, uint _lock, void* buffer)
	{
		if (!checkHResultReturn(cD3DSystem::getD3DDevice()->CreateIndexBuffer(size, usage, D3DFMT_INDEX16, pool, &m_stream, NULL)))
		{
			return false;
		}
		if (buffer)
		{
			void* data;
			if (!checkHResultReturn(lock(0, size, &data, _lock)))
			{
				return false;
			}
			memcpy(data, (void*)buffer, size);
			if (!checkHResultReturn(unlock()))
			{
				return false;
			}
		}
		return true;
	}
}