
#ifndef _BreadVBInfo9_h_
#define _BreadVBInfo9_h_

#include "BreadD3DHeader.h"
#include "BreadAnimationJoint.h"
#include "BreadFVFInfo.h"
#include "BreadVertexBuffer.h"

namespace Bread
{
#pragma pack(1)

	/*
	*/
	struct sLocalJointIndex
	{
		sLocalJointIndex() : m_index(NULL), m_num(0) {}
		~sLocalJointIndex() 
		{
			safeDeleteArray(m_index);
		}
		void operator = (sLocalJointIndex const& localJointIndex)
		{
			safeDeleteArray(m_index);
			m_num = localJointIndex.m_num;
			if (0 < m_num)
			{
				m_index = new WORD[m_num];
				memcpy(m_index, localJointIndex.m_index, sizeof (WORD) * m_num);
			}
		}
		WORD getWorldIndex(int localIndex) const	{	return (m_index) ? m_index[localIndex] : 0;	}
		int getJointNum() const						{	return m_num;	}

		WORD* m_index;
		int m_num;
	};
	/*
	*	
	*/
	struct sVBInfo
	{
		sVBInfo() : m_vbSize(0), m_stride(0), m_vertexNum(0), 
			m_decl(NULL), m_element(NULL), m_elementNum(0), m_localJointResult(NULL)
		{
			setFlag(false);
			//m_primitiveType = D3DPT_TRIANGLELIST;
			m_primitiveType = D3DPT_TRIANGLESTRIP;
		}
		~sVBInfo();
		void setFlag(bool dynamic);
		void setLocalJointResult(int localIndex, sMatrix4 const& mat);
		bool setFVFInfoToElement(int elementNum);
		bool setD3DFVFToElement(DWORD fvf);
		bool lock(void** data);
		bool unlock();
		WORD getWorldJointIndex(int localIndex) const	{	return m_localJointIndex.getWorldIndex(localIndex);	}
		int getLocalJointNum() const							{	return m_localJointIndex.getJointNum();					}
		template <typename T>
		T getVBOffset(void* vbList, uint offset, int fvf)
		{
			return reinterpret_cast<T>((char*)vbList + offset + m_fvfInfo[fvf].m_offset);
		}
		
		BUID m_buid;
		uint m_vbSize;
		uint m_stride;
		uint m_usage;
		D3DPOOL m_pool;
		uint m_lock;
		uint m_vertexNum;
		uint m_elementNum;
		sFVFInfo m_fvfInfo[sFVFInfo::FVF_NUM];
		D3DPRIMITIVETYPE m_primitiveType;
		sVertexBuffer m_vb;
		LPD3DVERTEXELEMENT9 m_element;
		LPDIRECT3DVERTEXDECLARATION9 m_decl;
		sLocalJointIndex m_localJointIndex;
		sMatrix4* m_localJointResult;
	};
	/*
	*	
	*/
	struct sVBCacheInfo
	{
		sVBCacheInfo() : m_buffer(NULL), m_bufferSize(0), m_stride(0), m_vertexNum(0),
							 m_decl(NULL), m_element(NULL), m_elementNum(0)
		{
			setFlag(false);
			m_primitiveType = D3DPT_TRIANGLESTRIP;
		}
		~sVBCacheInfo();
		void setFlag(bool dynamic);
		bool copy(sVBInfo* copyData, uint shareType);
		bool setFVFInfoToElement(int elementNum);
		template <typename T>
		T getVBOffset(void* vbList, uint offset, int fvf)
		{
			return reinterpret_cast<T>((char*)vbList + offset + m_fvfInfo[fvf].m_offset);
		}
		
		BUID m_buid;
		uint m_bufferSize;
		uint m_stride;
		uint m_usage;
		D3DPOOL m_pool;
		uint m_lock;
		uint m_vertexNum;
		uint m_elementNum;
		sFVFInfo m_fvfInfo[sFVFInfo::FVF_NUM];
		D3DPRIMITIVETYPE m_primitiveType;
		char* m_buffer;
		LPD3DVERTEXELEMENT9		m_element;
		LPDIRECT3DVERTEXDECLARATION9 m_decl;
		sLocalJointIndex m_localJointIndex;
	};

#pragma pack()
}


#endif