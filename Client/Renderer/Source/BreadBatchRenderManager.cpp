
#include "BreadBatchRenderManager.h"
#include "BreadDeclaration.h"
#include "BreadCore.h"
#include "BreadRenderPropertyManager.h"
#include "BreadRenderPrimitive.h"
#include "BreadVector3.h"
#include "BreadRenderData.h"
#include "BreadEntityModel.h"

namespace Bread
{
	implementInstance(cBatchRenderManager)

	uint cBatchRenderManager::m_lineListNum = 10000;
	uint cBatchRenderManager::m_lineStripNum = 10000;

	/*
	*/
	cBatchRenderManager::cBatchRenderManager() : m_lineList(NULL),
												 m_lineStrip(NULL),
												 m_primitiveLineListCount(0),
												 m_primitiveLineStripCount(0)
	{
		for (uint i = 0; i < MASK_NUM; ++i)
			m_entityList[i] = NULL;

		assignInstance(this)
	}
	/*
	*/
	cBatchRenderManager::~cBatchRenderManager()
	{		
		assignInstance(NULL)
	}
	/*
	*/
	void cBatchRenderManager::initialize()
	{
		initPrimitiveLineList();
		initPrimitiveLineStrip();

		for (uint i = 0; i < MASK_NUM; ++i)
			m_entityList[i] = new bucket_rd;
	}
	/*
	*/
	void cBatchRenderManager::finalize()
	{
		// line
		safeDeleteArray(m_lineList);
		safeDeleteArray(m_lineStrip);
		// entity
		for (uint i = 0; i < MASK_NUM; ++i)
			safeDelete(m_entityList[i]);
	}
	/*
	*/
	void cBatchRenderManager::clearList()
	{
		// line count
		m_primitiveLineListCount = 0;
		m_primitiveLineStripCount = 0;
		// entity
		for (uint i = 0; i < MASK_NUM; ++i)
		{
			if (m_entityList[i])
				m_entityList[i]->clear();
		}
	}
	/*
	*/
	void cBatchRenderManager::initPrimitiveLineList()
	{
		assert(!m_lineList);
		m_lineList = new sLineVertex[cBatchRenderManager::m_lineListNum];
	}
	/*
	*/
	void cBatchRenderManager::initPrimitiveLineStrip()
	{
		assert(!m_lineStrip);
		m_lineStrip = new sLineVertex[cBatchRenderManager::m_lineStripNum];
	}
	/*
	*/
	void cBatchRenderManager::update(float /*elapsedtime*/)
	{
	}
	/*
	*/
	void cBatchRenderManager::render3D()
	{
		renderLine();	
	}
	/*
	*/
	void cBatchRenderManager::render2D()
	{
	}
	/*
	*/
	void cBatchRenderManager::renderLine()
	{
		_getRenderPropertyMgr()->begin();
		_getRenderPropertyMgr()->setProperty(sRenderProperty::LIGHTING, FALSE);

		LPDIRECT3DDEVICE9 device = cD3DSystem::getD3DDevice();
		device->SetTexture(0, NULL);
		device->SetVertexShader(NULL);
		device->SetPixelShader(NULL);
		device->SetFVF(sLineVertex::D3DFVF);

		renderLineList();
		renderLineStrip();

		_getRenderPropertyMgr()->end();
	}
	/*
	*/
	void cBatchRenderManager::renderLineList()
	{
		if (0 < m_primitiveLineListCount)
			cRenderer::renderPrimitiveUP(D3DPT_LINELIST, m_primitiveLineListCount, (void*)m_lineList, sLineVertex::SIZE);
	}
	/*
	*/
	void cBatchRenderManager::renderLineStrip()
	{
		if (0 < m_primitiveLineStripCount)
			cRenderer::renderPrimitiveUP(D3DPT_LINESTRIP, m_primitiveLineStripCount, (void*)m_lineStrip, sLineVertex::SIZE);
	}
	/*
	*/
	void cBatchRenderManager::push(sLineVertex const* vertexList, unsigned int num)
	{
		/// 2개 단위가 아닐때는 맨 끝에것을 짤라 준다
		if (num%2 != 0)
		{
			assert(0 && _T("register lineVertex must be 2 unit"));
			num = (num/2) * 2;
		}

		int pos = m_primitiveLineListCount << 1;
		if (pos + num >= cBatchRenderManager::m_lineListNum)
		{
			num = cBatchRenderManager::m_lineListNum - pos;
		}

		for (unsigned int i = 0; i < num; i+=2, pos+=2)
		{
			m_lineList[pos+0] = vertexList[i+0];
			m_lineList[pos+1] = vertexList[i+1];
		}

		m_primitiveLineListCount += (num >> 1);
	}
	/*
	*/
	void cBatchRenderManager::push(sVector3 const* v1, sVector3 const* v2, DWORD color)
	{
		push(*v1, *v2, color);
	}
	/*
	*/
	void cBatchRenderManager::push(sVector3 const& v1, sVector3 const& v2, DWORD color)
	{
		unsigned int pos = m_primitiveLineListCount << 1;
		if (pos + 2 >= cBatchRenderManager::m_lineListNum)
		{
			return ;
		}

		m_lineList[pos+0] = sLineVertex(v1, color);
		m_lineList[pos+1] = sLineVertex(v2, color);

		m_primitiveLineListCount += 1;
	}
	/*
	*/
	void cBatchRenderManager::push(std::vector<sVector3> const& vertexList, DWORD color)
	{
		size_t num = vertexList.size();
		if (2 > num)
			return ;
		for (size_t n = 0; n < num-1; ++n)
		{
			push(vertexList[n], vertexList[n+1], color);
		}
	}
	/*
	*/
	void cBatchRenderManager::pushAtNormal(sVector3 const* ori, sVector3 const* normal, float normalScale, DWORD color)
	{
		unsigned int pos = m_primitiveLineListCount << 1;
		if (pos + 2 >= cBatchRenderManager::m_lineListNum)
		{
			return ;
		}

		sVector3 v2;
		mulVec3(*normal, normalScale, v2);
		v2 += *ori;

		m_lineList[pos+0] = sLineVertex(*ori, color);
		m_lineList[pos+1] = sLineVertex(v2, color);

		m_primitiveLineListCount += 1;
	}
	/*
	*/
	void cBatchRenderManager::pushLineStrip(sLineVertex const* /*lineVertex*/, unsigned int /*num*/)
	{
		/* 나중에 다시,,
		int pos = m_primitiveLineStripCount;

		for (unsigned int i = 0; i < num; ++i, ++pos)
		{
			m_lineStrip[pos+0] = lineVertex[i+0];
		}
		*/
	}
	/*
	*/
	void cBatchRenderManager::push(sBox const* box, DWORD color)
	{
		push(&box->m_v[0], &box->m_v[1], color);
		push(&box->m_v[1], &box->m_v[3], color);
		push(&box->m_v[3], &box->m_v[2], color);
		push(&box->m_v[2], &box->m_v[0], color);

		push(&box->m_v[4], &box->m_v[5], color);
		push(&box->m_v[5], &box->m_v[7], color);
		push(&box->m_v[7], &box->m_v[6], color);
		push(&box->m_v[6], &box->m_v[4], color);

		push(&box->m_v[0], &box->m_v[4], color);
		push(&box->m_v[1], &box->m_v[5], color);
		push(&box->m_v[2], &box->m_v[6], color);
		push(&box->m_v[3], &box->m_v[7], color);
	}
	/*
	*/
	void cBatchRenderManager::push(sSphere const* sphere, DWORD color)
	{
		//
		int vertNum = 50;
		float unitAngle = cMath::TWOPI/(float)vertNum;
		//
		sVector3 pos(1.0f, 0.0f, 0.0f);
		sVector3 out, startV, endV;
		addMulVec3(sphere->m_center, pos, sphere->m_radius, startV);
		//
		sQuaternion quat;
		float angle = 0.0f;
		for (int i = 0; i <= vertNum; ++i, angle += unitAngle)
		{
			rotQuat(pos, sVector3(0.0f, 1.0f, 0.0f), angle, out);
			addMulVec3(sphere->m_center, out, sphere->m_radius, endV);

			push(&startV, &endV, color);
			startV = endV;
		}
	}
	void cBatchRenderManager::push(cRenderData* rd, bool isSceneObject)
	{
		sEntityList rl;
		rl.m_rd = rd;

		m_entityList[MASK_ALL]->pushBack(rl);

		if (isSceneObject)
		{
			rd->m_entity->setRenderMask(MASK_SCENE);
			m_entityList[MASK_SCENE]->pushBack(rl);
		}
		else
		{
			rd->m_entity->setRenderMask(MASK_NOT_SCENE);
			m_entityList[MASK_NOT_SCENE]->pushBack(rl);
		}
	}
	/*
	 *	
	 */
	void cBatchRenderManager::renderEntityCullFigure(uint mask)
	{
		if (!isValidEntityList(mask))
			return ;

		struct sRenderCullFigure
		{
			void operator ()(sEntityList const& rl)
			{
				if (rl.m_rd)
					cRenderer::renderCullFigure(&rl.m_rd->m_cullFigure, 0xffffffff);
			}
		};

		m_entityList[mask]->forEach(sRenderCullFigure());
	}
	/*
	*/
	int cBatchRenderManager::getEntityListSize(uint mask) const
	{
		if (!isValidEntityList(mask))
			return 0;
		return m_entityList[mask]->getBucketSize();
	}
	/*
	*/
	bool cBatchRenderManager::isValidEntityList(uint mask) const
	{
		if (MASK_NUM <= mask)
			return false;
		if (!m_entityList[mask])
			return false;
		return true;
	}
}