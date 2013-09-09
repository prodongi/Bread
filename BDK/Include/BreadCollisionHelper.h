
#ifndef _BreadCollisionHelper_h_
#define _BreadCollisionHelper_h_

#include "BreadVector3.h"

namespace Bread
{
	/*
		@date 2012.
		@auth prodongi
		@desc 
		@todo 
	*/
	class cCollisionHelper
	{
	public:
		cCollisionHelper();
		~cCollisionHelper();

		/// @brief p에서 평면에 수선을 그었을 때의 최단 거리 구하는 함수, 리턴 값이 음수일 경우에는 p가 평면 뒤에 있다는 것이다.
		/// @param p 시작 위치, ps 평면의위 한 점, normal 평면의 법선 벡터
		static float perpendicularDistance(sVector3 const& p, sVector3 const& ps, sVector3 const& normal);
		/// @brief p에서 평면으로의 수선의 점을 구하는 함수
		static void perpendicularPoint(sVector3 const& p, sVector3 const& ps, sVector3 const& normal, sVector3& ret);
		/// @brief p에서 pf로 선을 그었을 때 평면과 만나는 교점을 구하는 함수
		/// @param p 시작 위치, pf 종료 위치, ps 평면위의 한 점, normal 평면의 법선 벡터
		static void intersectionPoint(sVector3 const& p, sVector3 const& pf, sVector3 const& ps, sVector3 const& normal, sVector3& ret);
		/// @brief p에서 pf로 선을 그었을 때 평면과 만나는 교점의 거리를 구하는 함수
		static float intersectionDistance(sVector3 const& p, sVector3 const& pf, sVector3 const& ps, sVector3 const& normal);
		/// @brief p에서 pf로 선을 그었을 때 평면의 반사 벡터를 구하는 함수
		/// @param p 시작 위치, pf 종료 위치, ps 평면위의 한 점, normal 평면의 법선 벡터, refPoint 반사 포인트, refVector 반사 벡터
		static void reflectionVector(sVector3 const& p, sVector3 const& pf, sVector3 const& ps, sVector3 const& normal, sVector3& refPoint, sVector3& refVector);
		/// @brief p에서 pf로 이동 중, 평면과 충돌 할 때의 물러나기 벡터를 구하는 함수
		/// @param p 시작 위치, pf 종료 위치, ps 평면위의 한 점, normal 평면의 법선 벡터
		static void withDrawVector(sVector3 const& p, sVector3 const& pf, sVector3 const& ps, sVector3 const& normal, sVector3& ret);
	};
}



#endif