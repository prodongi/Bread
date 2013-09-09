
#ifndef _BreadVertexBuffer9_h_
#define _BreadVertexBuffer9_h_

#include "BreadD3DHeader.h"
#include "BreadDefinition.h"

namespace Bread
{
	struct sVertexBuffer
	{
		sVertexBuffer();
		void release();
		bool is() const { return m_stream ? true : false; }
		LPDIRECT3DVERTEXBUFFER9 getStream() const {	return m_stream; }
		bool lock(uint offset, uint size, void** data, DWORD _lock);
		bool unlock();
		bool createStream(uint size, DWORD usage, DWORD fvf, D3DPOOL pool, uint _lock, void* buffer = NULL);

	private:
		LPDIRECT3DVERTEXBUFFER9 m_stream;
	};
}

#endif