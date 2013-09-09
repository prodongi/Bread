
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

		/// @brief p���� ��鿡 ������ �׾��� ���� �ִ� �Ÿ� ���ϴ� �Լ�, ���� ���� ������ ��쿡�� p�� ��� �ڿ� �ִٴ� ���̴�.
		/// @param p ���� ��ġ, ps ������� �� ��, normal ����� ���� ����
		static float perpendicularDistance(sVector3 const& p, sVector3 const& ps, sVector3 const& normal);
		/// @brief p���� ��������� ������ ���� ���ϴ� �Լ�
		static void perpendicularPoint(sVector3 const& p, sVector3 const& ps, sVector3 const& normal, sVector3& ret);
		/// @brief p���� pf�� ���� �׾��� �� ���� ������ ������ ���ϴ� �Լ�
		/// @param p ���� ��ġ, pf ���� ��ġ, ps ������� �� ��, normal ����� ���� ����
		static void intersectionPoint(sVector3 const& p, sVector3 const& pf, sVector3 const& ps, sVector3 const& normal, sVector3& ret);
		/// @brief p���� pf�� ���� �׾��� �� ���� ������ ������ �Ÿ��� ���ϴ� �Լ�
		static float intersectionDistance(sVector3 const& p, sVector3 const& pf, sVector3 const& ps, sVector3 const& normal);
		/// @brief p���� pf�� ���� �׾��� �� ����� �ݻ� ���͸� ���ϴ� �Լ�
		/// @param p ���� ��ġ, pf ���� ��ġ, ps ������� �� ��, normal ����� ���� ����, refPoint �ݻ� ����Ʈ, refVector �ݻ� ����
		static void reflectionVector(sVector3 const& p, sVector3 const& pf, sVector3 const& ps, sVector3 const& normal, sVector3& refPoint, sVector3& refVector);
		/// @brief p���� pf�� �̵� ��, ���� �浹 �� ���� �������� ���͸� ���ϴ� �Լ�
		/// @param p ���� ��ġ, pf ���� ��ġ, ps ������� �� ��, normal ����� ���� ����
		static void withDrawVector(sVector3 const& p, sVector3 const& pf, sVector3 const& ps, sVector3 const& normal, sVector3& ret);
	};
}



#endif