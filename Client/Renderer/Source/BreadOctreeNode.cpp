
#include "BreadOctreeNode.h"
#include "BreadUtils.h"
#include "BreadFrustum.h"
#include "BreadEntityModel.h"
#include "BreadCore.h"
#include "BreadSceneManager.h"
#include "BreadStringHelper.h"
#include "BreadProfile.h"
#include "BreadBatchRenderManager.h"

namespace Bread
{
	/*
	 *	
	 */
	uint cOctreeNode::m_renderColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	/*
	*	
	*/
	cOctreeNode::cOctreeNode(cOctreeNode* parent) : m_parent(parent)
	{
		for (int i = 0; i < REGION_NUM; ++i)
			m_node[i] = NULL;
	}
	/*
	 *	
	 */
	cOctreeNode::cOctreeNode() : m_parent(NULL)
	{
		for (int i = 0; i < REGION_NUM; ++i)
			m_node[i] = NULL;
	}
	/*
	*	
	*/
	cOctreeNode::~cOctreeNode()
	{
		for (int i = 0; i < REGION_NUM; ++i)
		{
			if (m_node[i])
				delete m_node[i];
		}
		m_rdList.clear();
	}
	/*
	*	
	*/
	void cOctreeNode::create(int depth, sBox const* in_bound)
	{
		m_depth = depth;
		memcpy(&m_bound, in_bound, sizeof (sBox));
	}
	/*
	*	
	*/
	bool cOctreeNode::add(cRenderData* renderData, sBox const* in_bound, float k, int maxDepth)
	{
		if (-1 != maxDepth && m_depth < maxDepth)
		{
			for (int i = 0; i < REGION_NUM; ++i)
			{
				if (isIn(i, in_bound, k))
				{
					if (!m_node[i])
					{
						sBox bound;
						getRegion(i, &bound);
						m_node[i] = new cOctreeNode(this);
						m_node[i]->create(m_depth+1, &bound);
					}
					m_node[i]->add(renderData, in_bound, k, maxDepth);
					return true;
				}
			}
		}

		if (renderData->m_sceneOctreeNode)
			renderData->m_sceneOctreeNode->del(renderData->m_buid);
		renderData->m_sceneOctreeNode = this;
		if (renderData->m_entity)
			m_rdList.insert(std::make_pair(renderData->m_buid, renderData->m_entity->getIsSceneObject()));

		return true;
	}
	/*
	*	
	*/
	bool cOctreeNode::add(cRenderData* renderData, sBox const* in_bound, bool isSceneObject, int maxDepth)
	{
		if (-1 != maxDepth && m_depth < maxDepth)
		{
			for (int i = 0; i < REGION_NUM; ++i)
			{
				if (isIn(i, in_bound))
				{
					if (!m_node[i])
					{
						sBox bound;
						getRegion(i, &bound);
						m_node[i] = new cOctreeNode(this);
						m_node[i]->create(m_depth+1, &bound);
					}
					m_node[i]->add(renderData, in_bound, isSceneObject, maxDepth);
					return true;
				}
			}
		}

		if (renderData->m_sceneOctreeNode)
			renderData->m_sceneOctreeNode->del(renderData->m_buid);
		renderData->m_sceneOctreeNode = this;
		m_rdList.insert(std::make_pair(renderData->m_buid, isSceneObject));

		return true;
	}
	/*
	*	
	*/
	void cOctreeNode::getRegion(int region, sBox* bound)
	{
		float cx = (m_bound.minx() + m_bound.maxx()) * 0.5f;
		float cy = (m_bound.miny() + m_bound.maxy()) * 0.5f;
		float cz = (m_bound.minz() + m_bound.maxz()) * 0.5f;

		switch (region)
		{
		case LBD:	bound->set(m_bound.minx(), cx, m_bound.miny(), cy, m_bound.minz(), cz);	break;
		case RBD:	bound->set(cx, m_bound.maxx(), m_bound.miny(), cy, m_bound.minz(), cz);	break;
		case LTD:	bound->set(m_bound.minx(), cx, m_bound.miny(), cy, cz, m_bound.maxz());	break;
		case RTD:	bound->set(cx, m_bound.maxx(), m_bound.miny(), cy, cz, m_bound.maxz());	break;
		case LBU:	bound->set(m_bound.minx(), cx, cy, m_bound.maxy(), m_bound.minz(), cz);	break;
		case RBU:	bound->set(cx, m_bound.maxx(), cy, m_bound.maxy(), m_bound.minz(), cz);	break;
		case LTU:	bound->set(m_bound.minx(), cx, cy, m_bound.maxy(), cz, m_bound.maxz());	break;
		case RTU:	bound->set(cx, m_bound.maxx(), cy, m_bound.maxy(), cz, m_bound.maxz());	break;
		}
	}
	/*
	*	
	*/
	bool cOctreeNode::isIn(int region, sBox const* in_bound, float k)
	{
		sBox bound;
		getRegion(region, &bound);
		bound.expand(k);
		return bound.isIn(*in_bound);
	}
	/*
	*	
	*/
	bool cOctreeNode::isIn(int region, sBox const* in_bound)
	{
		sBox bound;
		getRegion(region, &bound);
		return bound.isIn(*in_bound);
	}
	/*
	 *	
	 */
	void cOctreeNode::copy(cOctreeNode const* srcData)
	{
		if (!srcData)
			return ;

		m_depth = srcData->m_depth;
		m_bound = srcData->m_bound;
		m_parent = srcData->m_parent;
		for (int i = 0; i < REGION_NUM; ++i)
		{
			if (srcData->m_node[i])
			{
				m_node[i] = new cOctreeNode(srcData->getParent());
				m_node[i]->copy(srcData->m_node[i]);
			}
		}
		m_rdList = srcData->m_rdList;
	}
	/*
	 *	
	 */
	void cOctreeNode::expandK(float k)
	{
		m_bound.expand(k);

		for (int i = 0; i < REGION_NUM; ++i)
		{
			if (m_node[i])
			{
				m_node[i]->expandK(k);
			}
		}
	}
	/*
	 *	
	 */
	void cOctreeNode::culling(cFrustum const* frustum)
	{
		bool isEntireVisible = false;

		if (frustum->isVisible(m_bound, isEntireVisible))
		{
			it_rdlist it = m_rdList.begin();
			for (; it != m_rdList.end(); ++it)
			{
				cRenderData* rd = _getSceneMgr()->getRenderData(it->first);
				if (!rd)
					continue;
				if (isEntireVisible || frustum->isVisible(rd->m_cullFigure.m_sphere))
					_getBatchRenderMgr()->push(rd, it->second);
			}
		}
		else
		{
			return ;
		}

		if (isEntireVisible)
		{
			for (int i = 0; i < REGION_NUM; ++i)
			{
				if (m_node[i])
					m_node[i]->notculling();
			}
		}
		else
		{
			for (int i = 0; i < REGION_NUM; ++i)
			{
				if (m_node[i])
					m_node[i]->culling(frustum);
			}
		}
	}
	/*
	*/
	void cOctreeNode::notculling()
	{
		it_rdlist it = m_rdList.begin();
		for (; it != m_rdList.end(); ++it)
		{
			cRenderData* rd = _getSceneMgr()->getRenderData(it->first);
			if (!rd)
				continue;
			_getBatchRenderMgr()->push(rd, it->second);
		}

		for (int i = 0; i < REGION_NUM; ++i)
		{
			if (m_node[i])
				m_node[i]->notculling();
		}
	}
	/*
	 *	
	 */
	void cOctreeNode::add(cOctreeNode const* /*node*/)
	{
	}
	/*
	 *	
	 */
	void cOctreeNode::read(cSerializeBase* serialize)
	{
		serialize->readValue(m_depth);
		serialize->readValue(m_bound);

		TCHAR rdName[NAME_LEN];
		int rdNum;
		int isSceneObject;
		serialize->readValue(rdNum);
		for (int i = 0; i < rdNum; ++i)
		{
			serialize->readName(rdName);
			serialize->readValue(isSceneObject);

			m_rdList.insert(std::make_pair(rdName, (isSceneObject == 1) ? true : false));
		}

		int is;
		for (int i = 0; i < REGION_NUM; ++i)
		{
			serialize->readValue(is);
			if (1 == is)
			{
				m_node[i] = new cOctreeNode(this);
				m_node[i]->read(serialize);
			}
			else
			{
				m_node[i] = NULL;
			}
		}
	}
	/*
	 *	
	 */
	void cOctreeNode::reconstruct(cRenderData* renderData, sBox const* box)
	{
		if (m_bound.isIn(*box))
		{
			add(renderData, box, 1.0f);
		}
		else
		{
			if (m_parent)
				m_parent->reconstruct(renderData, box);
		}
	}
	/*
	 *	
	 */
	void cOctreeNode::del(BUID const& buid)
	{
		m_rdList.erase(buid);
	}
}


















