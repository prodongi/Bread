
#ifndef _BreadLine_h_
#define _BreadLine_h_

#include "BreadVector3.h"

namespace Bread
{

	struct sLine
	{
	public:
		sLine()		{}
		sLine(const sVector3& origin, const sVector3& direction)
		{
			m_origin = origin;
			m_direction = direction;
		}
		sLine(const sLine& line)
		{
			*this = line;
		}
		~sLine()	{}

		/**
		 *
		 * @brief m_origin���� �Ÿ� d��ŭ ������ line���� ����Ʈ
		 */
		inline void p1(float d, sVector3& out) const
		{
			addMulVec3(m_origin, m_direction, d, out);
		}
		/**
		 * @brief v�� line ���� �Ÿ��� ���Ѵ�, ��Ÿ��� ���� �̿�
		 */
		inline float distance(sVector3 const& p) const
		{
			sVector3 w;
			subVec3(p, m_origin, w);
			float v_sqr = m_direction.dot(m_direction);
			float w_sqr = w.dot(w);
			float proj = w.dot(m_direction);
			float inv = cMath::inv(v_sqr);
			return w_sqr - (proj * proj) * inv;
		}
		/**
		 *
		 * @brief v�� ���ϴ� norm ���
		 */
		inline void norm(sVector3 const& v, sVector3& out) const
		{
			sVector3 v1;
			subVec3(v, m_origin, v1);

			sVector3 v2;
			p1(1.0f, v2);

			sVector3 n1;
			v2.cross(v1, n1);
			v2.cross(n1, out);
			out.normalize();
		}
		/**
		 *
		 * @brief v1�� v2�� ������ ���� �������� �ִ��� üũ
		 */
		inline bool layLine(const sVector3& v1, const sVector3& v2) const
		{
			sVector3 cv1;
			subVec3(v1, m_origin, cv1);
			sVector3 cv2;
			subVec3(v2, m_origin, cv2);
			sVector3 c;
			cv1.cross(cv2, c);
			return c.length() < cMath::EPSILON;
		}
		/**
		 * @param p�� ���� ����� line���� �� ���ϱ�
		 */
		inline void closestPoint(sVector3 const& p, sVector3& out)
		{
			sVector3 w;
			subVec3(p, m_origin, w);
			sVector3 proj;
			p.proj(m_direction, proj);
			addVec3(m_origin, proj, out);
		}
		/**
		 *	 @param ���� ���ΰ� line���� ���� ����� p1, p2�� ���Ѵ�
		 */
		inline void closestPoints(sVector3& p1, sVector3& p2, const sLine& line)
		{
			sVector3 w0;
			subVec3(m_origin, line.m_origin, w0);
			float a = m_direction.dot(m_direction);
			float b = m_direction.dot(line.m_direction);
			float c = line.m_direction.dot(line.m_direction);
			float d = m_direction.dot(w0);
			float e = line.m_direction.dot(w0);

			float denom = a*c - b*b;
			if (cMath::isZero(denom))
			{
				p1 = m_origin;
				addMulVec3(line.m_origin, line.m_direction, e/c, p2);
			}
			else
			{
				addMulVec3(m_origin, m_direction, ((b*e - c*d)/denom), p1);
				addMulVec3(line.m_origin, line.m_direction, ((a*e - b*d)/denom), p2);
			}
		}
		/**
		 * @param �μ� ������ �Ÿ�(squared)	
		 */
		float distanceSqaured(const sLine& line)
		{
			sVector3 p1, p2;
			closestPoints(p1, p2, line);
			sVector3 p;
			subVec3(p1, p2, p);
			return p.dot(p);
		}


		inline sLine& operator = (const sLine& line)
		{
			m_origin = line.m_origin;
			m_direction = line.m_direction;

			return *this;
		}

	public:
		sVector3	m_origin;
		sVector3	m_direction;
	};
}











#endif











