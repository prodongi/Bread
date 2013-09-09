
#include "BreadAnimationController.h"
#include "BreadAnimationJoint.h"
#include "BreadAnimationMotionSet.h"
#include "BreadAnimationEvent.h"
#include "BreadVBManager.h"
#include "BreadMesh.h"
#include "BreadRenderPrimitive.h"
#include "BreadBatchRenderManager.h"
#include "BreadEntityModel.h"
#include "BreadQuaternion.h"
#include "BreadAnimationControlBox.h"
#include "BreadAnimationBlendTree.h"
#include "BreadAnimationBlendType.h"
#include "BreadAnimationBlendResult.h"
#include "BreadRenderData.h"

namespace Bread
{	
	/*
	*/
	cAnimationController::cAnimationController() : m_jointRef(NULL),
												   m_event(NULL),
												   m_jointNum(0),
												   m_controlBox(NULL),
												   m_blendTree(NULL),
												   m_curMotion(NULL)
	{
		setPause(false);
		setUseGpu(true);
	}
	/*
	*/
	cAnimationController::~cAnimationController()
	{
		finalize();
	}
	/*
	*/
	void cAnimationController::initialize(int jointNum, bool useGpu)
	{
		//
		m_jointNum = jointNum;
		m_useGpu = useGpu;
		
		//
		assert(!m_jointRef);
		m_jointRef = new sJointRef[m_jointNum];
		//
		m_blendTree = new cAnimationBlendTree;
		m_blendTree->initialize(m_jointNum);
	}
	/*
	*/
	void cAnimationController::copy(cAnimationController const* srcData)
	{
		initialize(srcData->m_jointNum, srcData->m_useGpu);

		for (int i = 0; i < m_jointNum; ++i)
			m_jointRef[i] = srcData->m_jointRef[i];
	}
	/*
	*/
	void cAnimationController::finalize()
	{
		safeDelete(m_blendTree, true);
		safeDeleteArray(m_jointRef);
	}
	/*
	*/
	void cAnimationController::update(float elapsedtime)
	{
		if (getPause())
			return ;

		m_blendTree->update(elapsedtime);
	}
	/*
	 *	
	 */
	void cAnimationController::setMotion(BUID const& motionBuid, cAnimationMotion* motion, cAnimationEvent* event)
	{
		m_curMotionBuid = motionBuid;
		m_curMotion = motion;
		m_blendTree->setMotion(m_curMotion, m_jointNum);
		
		m_event = event;
	}
	/*
	*/
#ifdef _DEBUG
	void cAnimationController::renderBone(sVector3 const& pos)
	{
		sAnimationResult const* lastResult = m_blendTree->getResult();
		if (!lastResult)
			return ;

		D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);
		sVector3 v0, v1;

		sVector3 tempPos;
		sQuaternion tempQuat;
		for (int i = 0; i < m_jointNum; ++i)
		{
			sJointRef const* jointRef = m_jointRef + i;
			int boneIndex = jointRef->m_index;
			if (0 <= boneIndex)
			{
				sJointResult* jointResult = lastResult->getJointResult(boneIndex);

				if (jointRef->m_parentIndex >= 0)
				{
					sJointResult const* resultParent = lastResult->getJointResult(jointRef->m_parentIndex);

					resultParent->getValue(tempPos, tempQuat);
					addVec3(pos, tempPos, v0);
					jointResult->getValue(tempPos, tempQuat);
					addVec3(pos, tempPos, v1);

					_getBatchRenderMgr()->push(&v0, &v1, color);			
				}
				else
				{
					jointResult->getValue(tempPos, tempQuat);
					addVec3(pos, tempPos, v1);

					_getBatchRenderMgr()->push(&v1, &v1, color);			
				}
			}
		}
	}
#else
	void cAnimationController::renderBone(sVector3 const& /*pos*/)
	{
	}
#endif
	/*
	*/
	void cAnimationController::skinning(cRenderData* renderData)
	{
		m_blendTree->blend(this);

		sAnimationResult const* lastResult = m_blendTree->getResult();
		if (!lastResult)
			return ;

		if (m_useGpu)
		{
			cRenderData::bucket_mesh* meshList = renderData->getMeshList();
			for (meshList->begin(); !meshList->isEnd(); meshList->next())
			{
				cMesh* mesh = meshList->getIterData();
				if (!mesh)
					continue;

				sVBInfo* vbInfo = _getVBMgr()->get(mesh->m_vbBuid);
				if (!vbInfo)
					continue;

				int localJointNum = vbInfo->getLocalJointNum();
				for (int i = 0; i < localJointNum; ++i)
				{
					int worldIndex = vbInfo->getWorldJointIndex(i);
					sJointResult const* joint = lastResult->getJointResult(worldIndex);
					joint->getValue(vbInfo->m_localJointResult[i]);
				}
			}
		}
		else
		{
			/*
			sJointResult* jointResult = m_result->m_joint;
			cRenderData::bucket_mesh* meshList = renderData->getMeshList();
			for (meshList->begin(); !meshList->isEnd(); meshList->next())
			{
				cMesh* mesh = meshList->getIterData();
				if (!mesh)
					continue;

				sVBInfo* vbInfo = _getVBMgr()->get(mesh->m_vbBuid);
				if (!vbInfo)
					continue;

				void* vbList;
				if (FAILED(vbInfo->m_vb->Lock(0, vbInfo->m_vbSize, (void**)&vbList, vbInfo->m_lock)))
					continue;

				uint offset = 0;
				sVector3 temp, v, n, temp2;
				for (uint vn = 0; vn < vbInfo->m_vertexNum; ++vn)
				{
					sVector3* position = vbInfo->getVBOffset<sVector3*>(vbList, offset, sFVFInfo::FVF_XYZ);
					sVector3* normal = vbInfo->getVBOffset<sVector3*>(vbList, offset, sFVFInfo::FVF_NORMAL);

					v = sVector3::ZERO;
					n = sVector3::ZERO;
					sPhysique* physique = vbInfo->m_physique + vn;
					for (WORD p = 0; p < physique->m_linkCount; ++p)
					{
						WORD jointIndex = vbInfo->getWorldJointIndex(physique->m_localIndex[p]);
						sJointResult* result = jointResult + jointIndex;

						// normal
						result->m_worldTRS.transformCoord(physique->m_normal, temp);
						mulVec3(temp, physique->m_weight[p], temp2);
						n += temp2;

						// vector
						mulMat4(result->m_worldTRS, physique->m_vertex, temp);
						mulVec3(temp, physique->m_weight[p], temp2);
						v += temp2;
					}

					*position = v;

					n.normalize();
					*normal = n;

					offset += vbInfo->m_stride;

					if (cRenderer::m_isRenderNormal)
					{
						sVector3 p;
						addVec3(renderData->m_entity->getPosition(), v, p);
						_getBatchRenderMgr()->pushLineListNormal(&p, &n, 1, 0xffff0000);
					}
				}

				vbInfo->m_vb->Unlock();
			}
			*/
		}
	}
	/*
	*/
	int cAnimationController::getJointRefBoneIndex(int jointIndex) const
	{
		if (0 > jointIndex && jointIndex >= m_jointNum)
		{
			assert(0 && _T("invalid joint num"));
			return NULL;
		}

		return m_jointRef[jointIndex].m_index;
	}
	/*
	*/
	sMatrix4 const& cAnimationController::getJointRefMatSkin(int jointIndex) const
	{
		if (0 > jointIndex && jointIndex >= m_jointNum)
		{
			assert(0 && _T("invalid joint num"));
			static sMatrix4 mat;
			return mat;
		}

		return m_jointRef[jointIndex].m_matSkin;
	}
}