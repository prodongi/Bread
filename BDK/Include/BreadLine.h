
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
		 * @brief m_origin에서 거리 d만큼 떨어진 line위의 포인트
		 */
		inline void p1(float d, sVector3& out) const
		{
			addMulVec3(m_origin, m_direction, d, out);
		}
		/**
		 * @brief v와 line 과의 거리를 구한다, 피타고라스 정리 이용
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
		 * @brief v로 항하는 norm 계산
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
		 * @brief v1과 v2가 현재의 동일 라인위에 있는지 체크
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
		 * @param p와 제일 가까운 line위의 점 구하기
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
		 *	 @param 현재 라인가 line에서 가장 가까운 p1, p2를 구한다
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
		 * @param 두선 사이의 거리(squared)	
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











