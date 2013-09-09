
#ifndef _BreadIndexBuffer9_h_
#define _BreadIndexBuffer9_h_

#include "BreadD3DHeader.h"
#include "BreadDefinition.h"

namespace Bread
{
	struct sIndexBuffer
	{
		sIndexBuffer();
		void release();
		bool is() const { return m_stream ? true : false; }
		LPDIRECT3DINDEXBUFFER9 getStream() const { return m_stream; }
		bool lock(uint offset, uint size, void** data, DWORD _lock);
		bool unlock();
		bool createStream(uint size, uint usage, D3DPOOL pool, uint _lock, void* buffer = NULL);

	private:
		LPDIRECT3DINDEXBUFFER9 m_stream;
	};
}

#endif