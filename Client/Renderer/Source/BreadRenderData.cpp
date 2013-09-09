
#include "BreadRenderData.h"
#include "BreadTextureManager.h"
#include "BreadRenderer.h"
#include "BreadOctreeNode.h"

namespace Bread
{
	/*
	 *	
	 */
	cRenderData::cRenderData(BUID buid) : m_buid(buid), m_sceneOctreeNode(NULL), m_entity(NULL)
	{
		m_mtlList = new bucket_mtl;
		m_meshList = new bucket_mesh;
	}
	/*
	 *	
	 */
	cRenderData::cRenderData() : m_sceneOctreeNode(NULL), m_entity(NULL)
	{
		m_mtlList = new bucket_mtl;
		m_meshList = new bucket_mesh;
	}
	/*
	 *	
	 */
	cRenderData::~cRenderData()
	{
		safeDelete(m_mtlList);
		safeDelete(m_meshList);
	}
	/*
	 *	
	 */
	void cRenderData::render()
	{
		// material list
		for (m_mtlList->begin(); !m_mtlList->isEnd(); m_mtlList->next())
		{
			cMaterial* mtl = m_mtlList->getIterData();

			_getTextureMgr()->setTexture(0, mtl->m_mapBuid[cMaterial::DIFFUSE]);

			// mesh list
			std::vector<std_string>::iterator it_mesh = mtl->m_meshList.begin();
			for (; it_mesh != mtl->m_meshList.end(); ++it_mesh)
			{
				cMesh* mesh = m_meshList->get(it_mesh->c_str());
				if (!mesh)
				{
					assert(0 && _T("Failed find mesh"));
					continue;
				}

				sVBInfo* vbInfo = _getVBMgr()->get(mesh->m_vbBuid);
				sIBInfo* ibInfo = _getIBMgr()->get(mesh->m_ibBuid);

				cRenderer::renderIndexedPrimitive(vbInfo, ibInfo);
			}
		}
	}
	/*
	 *	
	 */
	void cRenderData::setTranslation(sVector3 const& pos, sMatrix4 const& matRot)
	{
		m_cullFigure.setTranslation(pos, matRot);
		for (m_meshList->begin(); !m_meshList->isEnd(); m_meshList->next())
		{
			cMesh* mesh = m_meshList->getIterData();
			mesh->m_cullFigure.setTranslation(pos, matRot);
		}
		if (m_sceneOctreeNode)
			m_sceneOctreeNode->reconstruct(this, &m_cullFigure.m_box);
	}
	/*
	 *	
	 */
	void cRenderData::getMapBuidList(int mapType, std::vector<BUID>& list, bool first)
	{
		list.clear();
		for (m_mtlList->begin(); !m_mtlList->isEnd(); m_mtlList->next())
		{
			cMaterial* mtl = m_mtlList->getIterData();
			if (mtl && !mtl->m_mapBuid[mapType].empty())
			{
				list.push_back(mtl->m_mapBuid[mapType]);
				if (first) return ;
			}
		}
	}
	/*
	*/
	void cRenderData::copy(cRenderData* copyData, uint shareType)
	{
		copyData->m_buid = m_buid;
		m_cullFigure.copy(copyData->m_cullFigure);
		copyData->m_octree.copy(m_octree);
		m_mtlList->copy(*copyData->m_mtlList, shareType);
		m_meshList->copy(*copyData->m_meshList, shareType);
	}
}




















