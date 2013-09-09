
#ifndef _BreadBounding_h_
#define _BreadBounding_h_

#include "BreadVector3.h"

namespace Bread
{
	struct sMatrix4;

	/**
	*	
	*/
	struct sSphere
	{
		sSphere()
		{
			m_radius = 0.0f;
			m_center = sVector3::ZERO;
		}
		void setPosition(sSphere const& sphere, sVector3 const& pos)
		{
			addVec3(sphere.m_center, pos, m_center);
		}
		sSphere const& operator = (sSphere const& sphere)
		{
			m_radius = sphere.m_radius;
			m_center = sphere.m_center;
			return *this;
		}
		float		m_radius;
		sVector3	m_center;
	};
	/**
	*	
	*/
	struct sBox
	{
		enum {	VERT_NUM = 8	};
		sBox()
		{
			for (int i = 0; i < VERT_NUM; ++i)
				m_v[i] = sVector3::ZERO;
		}
		~sBox() {}
		void set(float minx, float maxx, float miny, float maxy, float minz, float maxz)
		{
			m_v[0] = sVector3(minx, maxy, minz);
			m_v[1] = sVector3(minx, maxy, maxz);
			m_v[2] = sVector3(maxx, maxy, minz);
			m_v[3] = sVector3(maxx, maxy, maxz);
			m_v[4] = sVector3(minx, miny, minz);
			m_v[5] = sVector3(minx, miny, maxz);
			m_v[6] = sVector3(maxx, miny, minz);
			m_v[7] = sVector3(maxx, miny, maxz);
		}
		template <typename T>
		void set(T const* value)
		{
			for (int i = 0; i < VERT_NUM; ++i)
				m_v[i] = value[i];
		}
		template <typename T>
		void get(T* value)
		{
			for (int i = 0; i < VERT_NUM; ++i)
				value[i] = m_v[i];
		}
		/// @brief 확장
		void expand(float k)
		{
			if (1.0f > k)
				return ;
			float scale = k - 1.0f;
			// 절반 길이
			float sizeX = (maxx() - minx()) * scale;
			float sizeY = (maxy() - miny()) * scale;
			float sizeZ = (maxz() - minz()) * scale;

			m_v[0] += sVector3(-sizeX, sizeY, -sizeZ);
			m_v[1] += sVector3(-sizeX, sizeY,  sizeZ);
			m_v[2] += sVector3(sizeX,  sizeY, -sizeZ);
			m_v[3] += sVector3(sizeX,  sizeY,  sizeZ);

			m_v[4] += sVector3(-sizeX, -sizeY, -sizeZ);
			m_v[5] += sVector3(-sizeX, -sizeY,  sizeZ);
			m_v[6] += sVector3(sizeX,  -sizeY, -sizeZ);
			m_v[7] += sVector3(sizeX,  -sizeY,  sizeZ);
		}
		template <typename T>
		bool isIn(T const& t)
		{
			if (t.maxx() <= maxx() && t.minx() >= minx() && 
				t.maxy() <= maxy() && t.miny() >= miny() &&
				t.maxz() <= maxz() && t.minz() >= minz())
				return true;
			return false;
		}
		template <>
		bool isIn<sVector3>(sVector3 const& t)
		{
			if (t.x >= minx() && t.x <= maxx() &&
				t.y >= miny() && t.y <= maxy() &&
				t.z >= minz() && t.z <= maxz())
				return true;
			return false;
		}
		/*
		*/
		void align()
		{
			sVector3 vmin = m_v[0];
			sVector3 vmax = m_v[0];
			for (int i = 0; i < sBox::VERT_NUM; ++i)
			{
				if (m_v[i].x < vmin.x)	vmin.x = m_v[i].x;
				if (m_v[i].y < vmin.y)	vmin.y = m_v[i].y;
				if (m_v[i].z < vmin.z)	vmin.z = m_v[i].z;
				if (m_v[i].x > vmax.x)	vmax.x = m_v[i].x;
				if (m_v[i].y > vmax.y)	vmax.y = m_v[i].y;
				if (m_v[i].z > vmax.z)	vmax.z = m_v[i].z;
			}

			set(vmin.x, vmax.x, vmin.y, vmax.y, vmin.z, vmax.z);
		}

		float minx() const	{	return m_v[0].x;	}
		float maxx() const	{	return m_v[2].x;	}
		float miny() const	{	return m_v[4].y;	}
		float maxy() const	{	return m_v[0].y;	}
		float minz() const	{	return m_v[0].z;	}
		float maxz() const	{	return m_v[1].z;	}
		float width() const	{	return cMath::fabsf(maxx()-minx());	}
		float height() const {	return cMath::fabsf(maxy()-miny());	}
		float depth() const	{	return cMath::fabsf(maxz()-minz());	}
		sVector3 const& maxv() const	{	return m_v[3];	}
		sVector3 const& minv() const	{	return m_v[4];	}
		void setTranslation(sBox const& box, sVector3 const& pos, sMatrix4 const& matRot);
		bool isZero() const
		{
			for (int i = 0; i < VERT_NUM; ++i)
			{
				if (!m_v[i].isZero())
					return false;
			}
			return true;
		}
		void calcCenter(sVector3& center) const
		{
			float halfW = width() * 0.5f;
			float halfH = height() * 0.5f;
			float halfZ = depth() * 0.5f;

			center.x = minx() + halfW;
			center.y = miny() + halfH;
			center.z = minz() + halfZ;
		}

		sBox const& operator = (sBox const& box)
		{
			memcpy(m_v, box.m_v, VERT_NUM * sizeof (sVector3));
			return *this;
		}
		// (dn) 5 7    (up) 1 3
		//      4 6         0 2
		sVector3 m_v[VERT_NUM];
	};
	/**
	*	
	*/
	struct sCullFigure
	{
		sSphere	m_sphere;
		sBox	m_box;

		void setTranslation(sVector3 const& pos, sMatrix4 const& matRot);
		void copyToBackup()
		{
			memcpy(&m_sphereBk, &m_sphere, sizeof (sSphere));
			memcpy(&m_boxBk, &m_box, sizeof (sBox));
		}
		sCullFigure const& operator = (sCullFigure const& cullFigure)
		{
			m_sphere = cullFigure.m_sphere;
			m_box = cullFigure.m_box;
			m_sphereBk = cullFigure.m_sphereBk;
			m_boxBk = cullFigure.m_boxBk;
			return *this;
		}
		void copy(sCullFigure& cullFigure)
		{
			memcpy(&cullFigure, this, sizeof (sCullFigure));
		}

	private:
		/// @brief backup용
		sSphere m_sphereBk;
		sBox	m_boxBk;
	};
}



#endif