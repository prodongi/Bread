
#ifndef _BreadUiPrimitive_h_
#define _BreadUiPrimitive_h_

#include "BreadD3DHeader.h"
#include "BreadVector3.h"

namespace Bread
{
	/*
	 *	
	 */
	struct sUiVertex
	{
		sUiVertex() : m_color(0xffffffff), m_rhw(1.0f) {}
		sUiVertex(sUiVertex const& iv)
		{
			memcpy(this, &iv, sizeof (sUiVertex));
		}
		void set(sVector3 const& pos, float tu, float tv, D3DCOLOR color)
		{
			m_position.x	= pos.x - 0.5f;
			m_position.y	= pos.y - 0.5f;
			m_position.z	= pos.z;
			m_tu			= tu;
			m_tv			= tv;
			m_color			= color;
			m_rhw			= 1.0f;
		}
		void setPosition(sVector3 const& pos)
		{
			m_position.x		= pos.x - 0.5f;
			m_position.y		= pos.y - 0.5f;
			m_position.z		= pos.z;
		}
		void operator = (sUiVertex const& iv)
		{
			memcpy(this, &iv, sizeof (sUiVertex));
		}
		void operator += (const sUiVertex& iv)
		{
			m_position += iv.m_position;
		}

		sVector3		m_position;
		float			m_rhw;
		D3DCOLOR		m_color;
		float			m_tu;
		float			m_tv;

		static DWORD D3DFVF;
		static size_t SIZE;
	};

	/*
	 *	
	 */
	struct sUiPlane
	{
		void set(float sx, float sy, float w, float h, D3DCOLOR color = 0xffffffff)
		{
			m_vertex[0].set(sVector3(sx, sy+h, 0.1f), 0.0f, 1.0f, color);
			m_vertex[1].set(sVector3(sx, sy, 0.1f), 0.0f, 0.0f, color);
			m_vertex[2].set(sVector3(sx+w, sy+h, 0.1f), 1.0f, 1.0f, color);
			m_vertex[3].set(sVector3(sx+w, sy, 0.1f), 1.0f, 0.0f, color);
		}

		sUiVertex m_vertex[4];

		static DWORD D3DFVF;
		static size_t VERTEX_SIZE;
	};
	/*
	 *	
	 */
	struct sUiT4Vertex
	{
		sUiT4Vertex() : m_rhw(1.0f) {}
		void set(sVector3 const& pos, float tu0, float tv0, 
										 float tu1, float tv1, 
										 float tu2, float tv2, 
										 float tu3, float tv3)
		{
			m_position.x = pos.x - 0.5f;
			m_position.y = pos.y - 0.5f;
			m_position.z = pos.z;
			m_uv[0][0] = tu0;	m_uv[0][1] = tv0;
			m_uv[1][0] = tu1;	m_uv[1][1] = tv1;
			m_uv[2][0] = tu2;	m_uv[2][1] = tv2;
			m_uv[3][0] = tu3;	m_uv[3][1] = tv3;
		}

		sVector3		m_position;
		float			m_rhw;
		float			m_uv[4][2];

		static DWORD D3DFVF;
		static size_t SIZE;
	};
	/*
	 *	
	 */
	struct sUiT4Plane
	{
		void set(float sx, float sy, float w, float h, float du, float dv)
		{
			m_vertex[0].set(sVector3(sx, sy+h, 0.1f)	, 0.0f, 1.0f, 0.0f+du, 1.0f, 0.0f, 1.0f+dv, 0.0f+du, 1.0f+dv);
			m_vertex[1].set(sVector3(sx, sy, 0.1f)		, 0.0f, 0.0f, 0.0f+du, 0.0f, 0.0f, 0.0f+dv, 0.0f+du, 0.0f+dv);
			m_vertex[2].set(sVector3(sx+w, sy+h, 0.1f)	, 1.0f, 1.0f, 1.0f+du, 1.0f, 1.0f, 1.0f+dv, 1.0f+du, 1.0f+dv);
			m_vertex[3].set(sVector3(sx+w, sy, 0.1f)	, 1.0f, 0.0f, 1.0f+du, 0.0f, 1.0f, 0.0f+dv, 1.0f+du, 0.0f+dv);
		}

		sUiT4Vertex m_vertex[4];

		static DWORD D3DFVF;
		static size_t VERTEX_SIZE;
	};
}








#endif