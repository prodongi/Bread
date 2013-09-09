
#ifndef _BreadAnimationJoint_h_
#define _BreadAnimationJoint_h_

#include "BreadDeclaration.h"
#include "BreadVector3.h"
#include "BreadMatrix4.h"
#include "BreadQuaternion.h"

namespace Bread
{
#pragma pack(1)

	/*
	*	레퍼런스 본 노드
	*/
	struct sJointRef
	{
		enum	{	MESH, BONE };

		sJointRef const& operator = (sJointRef const& jointRef)
		{
			memcpy(this, &jointRef, sizeof (sJointRef));

			return *this;
		}
		void copy(sJointRef const* srcData)
		{
			*this = *srcData;
		}

		TCHAR m_name[NAME_LEN];
		TCHAR m_parentName[NAME_LEN];
		int m_index;
		int m_parentIndex;
		int m_kind;
		sMatrix4 m_matSkin;	/// @brief inverse orientation TM matrix
	};
	/*
	*	
	*/
	struct sJoint
	{
		sVector3 m_pos;
		sQuaternion m_quat;
	};
	/*
	*	
	*/
	struct sAnimationFrame
	{
		sAnimationFrame() : m_joint(NULL) {}
		~sAnimationFrame()	{	safeDeleteArray(m_joint);	}
		void copy(sAnimationFrame const& srcData, int boneNodeNum)
		{
			m_joint = new sJoint[boneNodeNum];
			memcpy(m_joint, srcData.m_joint, sizeof (sJoint) * boneNodeNum);
		}

		sJoint* m_joint;
	};

#pragma pack()	
}



#endif
