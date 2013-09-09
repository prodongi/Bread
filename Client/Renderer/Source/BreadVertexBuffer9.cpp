
#include "BreadVertexBuffer9.h"
#include "BreadInline.h"
#include "BreadDebug.h"
#include "BreadD3DSystem.h"

namespace Bread
{
	/*
	*/
	sVertexBuffer::sVertexBuffer() : m_stream(NULL)
	{
	}
	/*
	*/
	void sVertexBuffer::release()
	{
		safeRelease(m_stream);
	}
	/*
	*/
	bool sVertexBuffer::lock(uint offset, uint size, void** data, DWORD _lock)
	{
		if (!checkHResultReturn(m_stream->Lock(offset, size, data, _lock)))
			return false;
		return true;
	}
	/*
	*/
	bool sVertexBuffer::unlock()
	{
		if (!checkHResultReturn(m_stream->Unlock()))
			return false;
		return true;
	}
	/*
	*/
	bool sVertexBuffer::createStream(uint size, DWORD usage, DWORD fvf, D3DPOOL pool, uint _lock, void* buffer)
	{
		if (!checkHResultReturn(cD3DSystem::getD3DDevice()->CreateVertexBuffer(size, usage, fvf, pool, &m_stream, NULL)))
		{
			return false;
		}
		if (buffer)
		{
			void* data;
			if (!lock(0, size, &data, _lock))
			{
				return false;
			}
			memcpy(data, buffer, size);
			if (!unlock())
			{
				return false;
			}
		}
		return true;
	}
}