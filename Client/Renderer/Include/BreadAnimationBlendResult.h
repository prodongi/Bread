
#ifndef _BreadAnimationBlendResult_h_
#define _BreadAnimationBlendResult_h_

#include "BreadDeclaration.h"
#include "BreadVector3.h"
#include "BreadMatrix4.h"
#include "BreadQuaternion.h"
#include "BreadBoostNormalPool.h"
#include "BreadCachePool.h"

namespace Bread
{
	class cAnimationController;

#pragma pack(1)

	/*
	*/
	struct sJointResult
	{
		virtual void copy(sJointResult const* /*joint*/) {}
		virtual void getValue(sVector3& /*pos*/, sQuaternion& /*quat*/) const {}
		virtual void getValue(sMatrix4& /*worldTRS*/) const {}
		virtual void set(sVector3 const& /*pos*/, sQuaternion const& /*quat*/) {}
		virtual void slerp(float /*blendRate*/, sJointResult const* /*startJoint*/, sJointResult const* /*endJoint*/) {}
		virtual void transformWorldTRS(sVector3 const& /*pos*/, sQuaternion const& /*quat*/, sMatrix4 const& /*matSkin*/) {}

	protected:
		sJointResult() {}
		sJointResult(sJointResult const& /*jointResult*/) {}
	};
	/*	/// @brief 블렌딩 계산 중간에 쓰임
	*/
	struct sJointInterResult : public sJointResult
	{
		declareCachePool()

		virtual void copy(sJointResult const* joint)
		{
			sJointInterResult const* j = dynamicCast<sJointInterResult const*>(joint);
			if (!j) return ;

			m_pos = j->m_pos;
			m_quat = j->m_quat;

		}
		virtual void getValue(sVector3& pos, sQuaternion& quat) const
		{
			pos = m_pos;
			quat = m_quat;
		}
		virtual void set(sVector3 const& pos, sQuaternion const& quat)
		{
			m_pos = pos;
			m_quat = quat;
		}
		virtual void slerp(float blendRate, sJointResult const* startJoint, sJointResult const* endJoint)
		{
			sJointInterResult const* sj = dynamicCast<sJointInterResult const*>(startJoint);
			if (!sj) return ;
			sJointInterResult const* ej = dynamicCast<sJointInterResult const*>(endJoint);
			if (!ej) return ;

			lerpVec3(sj->m_pos, ej->m_pos, blendRate, m_pos);
			m_quat.slerp(sj->m_quat, ej->m_quat, blendRate);
		}

		sVector3 m_pos;			/// @brief 블렌딩 계산할 때, 중간 결과 값에서 쓰임
		sQuaternion m_quat;		/// @brief 블렌딩 계산할 때, 중간 결과 값에서 쓰임

	};
	/* /// @brief 블렌딩 최종 값에 쓰임
	*/
	struct sJointLastResult : public sJointResult
	{
		declareCachePool()

		virtual void getValue(sMatrix4& worldTRS) const
		{
			worldTRS = m_worldTRS;
		}
		/// m_worldTRS 계산
		void transformWorldTRS(sVector3 const& pos, sQuaternion const& quat, sMatrix4 const& matSkin)
		{
			static sMatrix4 matTemp;

			quat.toMatrix(matTemp);
			matTemp.translation(pos);

			mulMat4(matSkin, matTemp, m_worldTRS);
		}

		sMatrix4 m_worldTRS;	/// @brief translation, rotation, scale된 행렬
	};
	/*
	 *	
	 */
	struct sAnimationResult
	{
		virtual ~sAnimationResult() {}
		virtual sJointResult* getJointResult(int /*index*/) const pure;
		virtual void createJoint(int /*jointNum*/) pure;
		virtual void copyWorldTRS(sAnimationResult const* /*result*/, int /*jointNum*/, cAnimationController const* /*controller*/) {}
		virtual void slerpWorldTRS(float /*blendRate*/, sAnimationResult const* /*startResult*/, sAnimationResult const* /*endResult*/, 
								   int /*jointNum*/, cAnimationController const* /*contoller*/) {}

	protected:
		sAnimationResult() {}
		sAnimationResult(sAnimationResult const& /*result*/) {}
	};
	/*
	*/
	struct sAnimationInterResult : public sAnimationResult
	{
		declareNormalPool(sAnimationInterResult)

	public:
		sAnimationInterResult() : m_joint(NULL) {}
		virtual ~sAnimationInterResult()	{	safeDeleteArray(m_joint);	}
		virtual sJointResult* getJointResult(int index) const;
		virtual void createJoint(int jointNum);
		virtual void copyWorldTRS(sAnimationResult const* result, int jointNum, cAnimationController const* controller);
		virtual void slerpWorldTRS(float blendRate, sAnimationResult const* startResult, sAnimationResult const* endResult, int jointNum, cAnimationController const* contoller);

	protected:
		sJointInterResult* m_joint;
	};
	/*
	*/
	struct sAnimationLastResult : public sAnimationResult
	{
		declareNormalPool(sAnimationLastResult)

	public:
		sAnimationLastResult() : m_joint(NULL) {}
		virtual ~sAnimationLastResult()	{	safeDeleteArray(m_joint);	}
		virtual sJointResult* getJointResult(int index) const;
		virtual void createJoint(int jointNum);
		virtual void copyWorldTRS(sAnimationResult const* result, int jointNum, cAnimationController const* controller);
		virtual void slerpWorldTRS(float blendRate, sAnimationResult const* startResult, sAnimationResult const* endResult, int jointNum, cAnimationController const* contoller);

	protected:
		sJointLastResult* m_joint;
	};

#pragma pack()
}


#endif