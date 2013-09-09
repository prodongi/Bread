
#ifndef _BreadRenderPrimitive_h_
#define _BreadRenderPrimitive_h_

#include "BreadD3DHeader.h"
#include "BreadVector3.h"
#include "BreadVector2.h"

namespace Bread
{
	#pragma pack(1)

	/*
	*/
	struct sPointVertex
	{
		sPointVertex() {}
		sPointVertex(sVector3 const& v)
		{
			set(v.x, v.y, v.z);
		}
		void set(float x, float y, float z)
		{
			m_x = x;	m_y = y;	m_z = z;
		}
		void operator = (sPointVertex const& pointVertex)
		{
			m_x = pointVertex.m_x;
			m_y = pointVertex.m_y;
			m_z = pointVertex.m_z;
		}
		float m_x, m_y, m_z;

		static DWORD D3DFVF;
		static size_t SIZE;
	};
	/*
	 *	
	 */
	struct sLineVertex
	{
		sLineVertex()	{}
		sLineVertex(sVector3 const& v, DWORD color)
		{
			set(v.x, v.y, v.z, color);
		}
		void set(float x, float y, float z, DWORD color)
		{
			m_x = x; m_y = y; m_z = z;
			m_color = color;
		}
		void set(sVector3 const& v, DWORD color)
		{
			set(v.x, v.y, v.z, color);
		}
		void operator = (sLineVertex const& lineVertex)
		{
			memcpy(this, &lineVertex, sizeof (sLineVertex));
		}
		float m_x, m_y, m_z;
		DWORD m_color;

		static DWORD D3DFVF;
		static size_t SIZE;
	};
	/*
	*/
	struct sUvVertex
	{
		sUvVertex()	{}
		sUvVertex(sVector3 const& v, float _u, float _v)
		{
			set(v.x, v.y, v.z, _u, _v);
		}
		void set(float x, float y, float z, float u, float v)
		{
			m_x = x; m_y = y; m_z = z;
			m_u = u; m_v = v;
		}
		void operator = (sUvVertex const& uvVertex)
		{
			memcpy(this, &uvVertex, sizeof (sUvVertex));
		}
		float m_x, m_y, m_z;
		float m_u, m_v;

		static DWORD D3DFVF;
		static size_t SIZE;
	};
	/*
	*/
	struct sUvColorVertex
	{
		sUvColorVertex()	{}
		sUvColorVertex(sVector3 const& v, sVector2 const& uv, DWORD color)
		{
			set(v, uv, color);
		}
		void set(float x, float y, float z, float u, float v, DWORD color)
		{
			m_x = x; m_y = y; m_z = z;
			m_u = u; m_v = v;
			m_color = color;
		}
		void set(sVector3 const& v, sVector2 const& uv, DWORD color)
		{
			set(v.x, v.y, v.z, uv.x, uv.y, color);
		}
		void operator = (sUvColorVertex const& vertex)
		{
			memcpy(this, &vertex, sizeof (sUvColorVertex));
		}
		float m_x, m_y, m_z;
		DWORD m_color;
		float m_u, m_v;

		static DWORD D3DFVF;
		static size_t SIZE;
	};
	/*
	 *	
	 */
	struct sFaceVertex
	{
		void set(float x, float y, float z, float nx, float ny, float nz, float u, float v, DWORD color)
		{
			m_x = x; m_y = y; m_z = z;
			m_nx = nx; m_ny = ny; m_nz = nz;
			m_u = u; m_v = v;
			m_color = color;
		}
		float m_x, m_y, m_z;
		float m_nx, m_ny, m_nz;
		DWORD m_color;
		float m_u, m_v;

		static DWORD D3DFVF;
		static size_t SIZE;
	};
	/*
	*/
	struct sVolumeCloudVertex
	{
		sVolumeCloudVertex(float x, float y, float z, DWORD color, float u, float v, float s)
		{
			m_x = x;
			m_y = y;
			m_z = z;
			m_color = color;
			m_u = u;
			m_v = v;
			m_s = s;
		}
		void set(float x, float y, float z, DWORD color, float u, float v, float s)
		{
			m_x = x;
			m_y = y;
			m_z = z;
			m_color = color;
			m_u = u;
			m_v = v;
			m_s = s;
		}
		float m_x, m_y, m_z;
		DWORD m_color;
		float m_u, m_v, m_s;

		static DWORD D3DFVF;
		static size_t SIZE;
	};

	#pragma pack()
}











#endif