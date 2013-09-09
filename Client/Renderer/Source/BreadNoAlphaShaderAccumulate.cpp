
#include "BreadNoAlphaShaderAccumulate.h"
#include "BreadMaterial.h"
#include "BreadMesh.h"
#include "BreadRenderPropertyManager.h"
#include "BreadSceneManager.h"
#include "BreadEntityFadeManager.h"
#include "BreadD3DDeclaration.h"
#include "BreadBatchRenderManager.h"
#include "BreadRenderDataManager.h"
#include "BreadUberShaderManager.h"
#include "BreadAlphaAccumulate.h"

namespace Bread
{
	/*
	*/
	cNoAlphaShaderAccumulate::cNoAlphaShaderAccumulate()
	{
	}
	/*
	*/
	cNoAlphaShaderAccumulate::~cNoAlphaShaderAccumulate()
	{
		sMeshCollector::pergeMemory();
		sMtlCollector::pergeMemory();
		sShaderCollector::pergeMemory();
	}
	/*
	*/
	void cNoAlphaShaderAccumulate::clear()
	{
		safeClearMapPtr_R<iter_shadercol>(m_list);
	}
	/*
	*/
	void cNoAlphaShaderAccumulate::collect(cRenderData* renderData, cMaterial const* mtl, cAlphaAccumulate* alphaAccum)
	{
		sShaderCollector* sc;
		sMtlCollector* mtlc;
		sMeshCollector* meshc;

		// find shader collector
		iter_shadercol it_sh_ = m_list.find(mtl->m_shaderBuid);
		if (it_sh_ == m_list.end())
		{
			sc = new sShaderCollector;
			m_list.insert(std::make_pair(mtl->m_shaderBuid, sc));
		}
		else
		{
			sc = it_sh_->second;
		}

		// find material collector
		iter_mtlcol it_mtl_ = sc->m_mtlColList.find(mtl->m_buid);
		if (it_mtl_ == sc->m_mtlColList.end())
		{
			mtlc = new sMtlCollector;
			sc->m_mtlColList.insert(std::make_pair(mtl->m_buid, mtlc));
		}
		else
		{
			mtlc = it_mtl_->second;
		}

		mtlc->m_mtl = mtl;

		cMaterial::citer_mesh mtl_mesh = mtl->m_meshList.begin();
		for (; mtl_mesh != mtl->m_meshList.end(); ++mtl_mesh)
		{
			if (_getEntityFadeMgr()->is(renderData->m_entity->getBuid()))
			{
				alphaAccum->collectMesh(renderData, mtl, *mtl_mesh);
			}
			else
			{
				meshc = new sMeshCollector;
				meshc->m_entity = renderData->m_entity;
				meshc->m_mesh = renderData->getMesh(*mtl_mesh);
				mtlc->m_meshColList.push_back(meshc);
			}
		}
	}
	/*
	*/
	void cNoAlphaShaderAccumulate::collectEmptyMtl(cRenderData* renderData, cAlphaAccumulate* alphaAccum)
	{
		sShaderCollector* sc;
		sMtlCollector* mtlc;
		sMeshCollector* meshc;

		cMaterial const* mtl = cMaterial::getEmptyMtl();
		
		// find shader collector
		iter_shadercol it_sh_ = m_list.find(mtl->m_shaderBuid);
		if (it_sh_ == m_list.end())
		{
			sc = new sShaderCollector;
			m_list.insert(std::make_pair(mtl->m_shaderBuid, sc));
		}
		else
		{
			sc = it_sh_->second;
		}

		// find material collector
		iter_mtlcol it_mtl_ = sc->m_mtlColList.find(mtl->m_buid);
		if (it_mtl_ == sc->m_mtlColList.end())
		{
			mtlc = new sMtlCollector;
			sc->m_mtlColList.insert(std::make_pair(mtl->m_buid, mtlc));
		}
		else
		{
			mtlc = it_mtl_->second;
		}

		mtlc->m_mtl = mtl;

		cRenderData::bucket_mesh* meshList = renderData->getMeshList();
		for (meshList->begin(); !meshList->isEnd(); meshList->next())
		{
			cMesh* mesh = meshList->getIterData();
			if (!mesh)
				continue;

			if (_getEntityFadeMgr()->is(renderData->m_entity->getBuid()))
			{
				alphaAccum->collectMesh(renderData, mtl, mesh->m_buid);
			}
			else
			{
				meshc = new sMeshCollector;
				meshc->m_entity = renderData->m_entity;
				meshc->m_mesh = renderData->getMesh(mesh->m_buid);
				mtlc->m_meshColList.push_back(meshc);
			}
		}
	}
	/*
	*/
	void cNoAlphaShaderAccumulate::render()
	{
		/*
		*	shader
		*/
		iter_shadercol it_shader = m_list.begin();
		for (; it_shader != m_list.end(); ++it_shader)
		{
			cUberShader* shader = _getUberShaderMgr()->get(it_shader->first);
			if (!shader)
			{
				shader = _getUberShaderMgr()->getBase();
				if (!shader)
					continue;
			}

			shader->beginRenderProperty();

			/*
			*	material
			*/
			map_mtlcol& mtlList = it_shader->second->m_mtlColList;
			iter_mtlcol it_mtl = mtlList.begin();
			for (; it_mtl != mtlList.end(); ++it_mtl)
			{
				shader->setMaterialParam(it_mtl->second->m_mtl);

				/*
				*	mesh
				*/
				uint passes;
				shader->begin(&passes);

				vec_meshcol& meshList = it_mtl->second->m_meshColList;
				iter_meshcol it_mesh = meshList.begin();
				for (; it_mesh != meshList.end(); ++it_mesh)
				{
					cMesh* mesh = (*it_mesh)->m_mesh;

					if (!mesh)
					{
						//assert(0 && _T("mesh is NULL"));
						continue;
					}

					shader->setMeshParam(mesh, (*it_mesh)->m_entity);

					for (uint p = 0; p < passes; ++p)
					{
						shader->beginPass(p);

						sVBInfo* vbInfo = _getVBMgr()->get(mesh->m_vbBuid);
						sIBInfo* ibInfo = _getIBMgr()->get(mesh->m_ibBuid);

						cRenderer::renderIndexedPrimitive(vbInfo, ibInfo);

						shader->endPass();
					}
				}
				shader->end();				
			}
			shader->endRenderProperty();
		}
	}
	/*
	*/
	void cNoAlphaShaderAccumulate::renderCullFigure()
	{
		if (!_getIsRender(RENDER_MESH_CULLFIGURE))
			return ;

		_getRenderPropertyMgr()->begin();
		_getRenderPropertyMgr()->setProperty(sRenderProperty::ZENABLE, D3DZB_FALSE);

		DWORD color = D3DCOLOR_ARGB(255, 255, 255, 0);

		iter_shadercol it_shader = m_list.begin();
		for (; it_shader != m_list.end(); ++it_shader)
		{
			map_mtlcol& mtlList = it_shader->second->m_mtlColList;
			iter_mtlcol it_mtl = mtlList.begin();
			for (; it_mtl != mtlList.end(); ++it_mtl)
			{
				vec_meshcol& meshList = it_mtl->second->m_meshColList;
				iter_meshcol it_mesh = meshList.begin();
				for (; it_mesh != meshList.end(); ++it_mesh)
				{
					cMesh* mesh = (*it_mesh)->m_mesh;
					cRenderer::renderCullFigure(&mesh->m_cullFigure, color);
				}		
			}
		}

		_getRenderPropertyMgr()->end();
	}
	/*
	*/
	void cNoAlphaShaderAccumulate::update(float elapsedtime)
	{
		/// shader
		iter_shadercol it_shader = m_list.begin();
		for (; it_shader != m_list.end(); ++it_shader)
		{
			cUberShader* shader = _getUberShaderMgr()->get(it_shader->first);
			if (!shader)
				continue;

			/// material
			map_mtlcol& mtlList = it_shader->second->m_mtlColList;
			iter_mtlcol it_mtl = mtlList.begin();
			for (; it_mtl != mtlList.end(); ++it_mtl)
			{
				/// mesh
				vec_meshcol& meshList = it_mtl->second->m_meshColList;
				iter_meshcol it_mesh = meshList.begin();
				for (; it_mesh != meshList.end(); ++it_mesh)
				{
					shader->update(elapsedtime, (*it_mesh)->m_mesh, it_mtl->second->m_mtl, (*it_mesh)->m_entity);
				}			
			}
		}

		if (_getIsRender(RENDER_NORMAL) || _getIsRender(RENDER_TANGENT) || _getIsRender(RENDER_BINORMAL))
		{
			updateDebugInfo();
		}
	}
	/*
	*/
	void cNoAlphaShaderAccumulate::updateDebugInfo()
	{
		iter_shadercol it_shader = m_list.begin();
		for (; it_shader != m_list.end(); ++it_shader)
		{
			/*
			*	material
			*/
			map_mtlcol& mtlList = it_shader->second->m_mtlColList;
			iter_mtlcol it_mtl = mtlList.begin();
			for (; it_mtl != mtlList.end(); ++it_mtl)
			{
				/*
				*	mesh
				*/
				vec_meshcol& meshList = it_mtl->second->m_meshColList;
				iter_meshcol it_mesh = meshList.begin();
				for (; it_mesh != meshList.end(); ++it_mesh)
				{
					cEntityModel* entity = (*it_mesh)->m_entity;
					cMesh* mesh = (*it_mesh)->m_mesh;
					sVBInfo* vbInfo = _getVBMgr()->get(mesh->m_vbBuid);
					if (!vbInfo)
						continue;

					void* vbList;
					if (!vbInfo->m_vb.lock(0, vbInfo->m_vbSize, (void**)&vbList, vbInfo->m_lock))
						continue;

					uint offset = 0;
					sVector3 temp, p;
					for (uint vn = 0; vn < vbInfo->m_vertexNum; ++vn)
					{
						sVector3* position = vbInfo->getVBOffset<sVector3*>(vbList, offset, sFVFInfo::FVF_XYZ);
						addVec3(*position, entity->getPosition(), p);
						
						if (_getIsRender(RENDER_NORMAL))
						{
							sVector3* normal = vbInfo->getVBOffset<sVector3*>(vbList, offset, sFVFInfo::FVF_NORMAL);
							entity->getMatRot().transformCoord(*normal, temp);
							_getBatchRenderMgr()->pushAtNormal(&p, &temp, 10.0f, COLOR_GREEN);
						}
						if (_getIsRender(RENDER_TANGENT))
						{
							sVector3* tangent = vbInfo->getVBOffset<sVector3*>(vbList, offset, sFVFInfo::FVF_TANGENT);
							entity->getMatRot().transformCoord(*tangent, temp);
							_getBatchRenderMgr()->pushAtNormal(&p, &temp, 10.0f, COLOR_RED);
						}
						if (_getIsRender(RENDER_BINORMAL))
						{
							sVector3* binormal = vbInfo->getVBOffset<sVector3*>(vbList, offset, sFVFInfo::FVF_BINORMAL);
							entity->getMatRot().transformCoord(*binormal, temp);
							_getBatchRenderMgr()->pushAtNormal(&p, &temp, 10.0f, COLOR_BLUE);
						}

						offset += vbInfo->m_stride;
					}

					vbInfo->m_vb.unlock();
				}			
			}
		}
	}
	/*
	*/
	void cNoAlphaShaderAccumulate::renderSingleShader(cShader* shader, std_string const& technique, uint renderMask)
	{
		uint passes;
		shader->setTechnique(technique.c_str());
		shader->setDefaultTexture();
		shader->begin(&passes);

		/*
		*	shader
		*/
		iter_shadercol it_shader = m_list.begin();
		for (; it_shader != m_list.end(); ++it_shader)
		{
			cUberShader* oriShader = _getUberShaderMgr()->get(it_shader->first);
			if (!oriShader)
				continue;

			/*
			*	material
			*/
			map_mtlcol& mtlList = it_shader->second->m_mtlColList;
			iter_mtlcol it_mtl = mtlList.begin();
			for (; it_mtl != mtlList.end(); ++it_mtl)
			{
				/*
				*	mesh
				*/
				vec_meshcol& meshList = it_mtl->second->m_meshColList;
				iter_meshcol it_mesh = meshList.begin();
				for (; it_mesh != meshList.end(); ++it_mesh)
				{
					cMesh* mesh = (*it_mesh)->m_mesh;

					if (!mesh)
					{
						//assert(0 && _T("mesh is NULL"));
						continue;
					}

					if (!(*it_mesh)->m_entity->isValidRenderMask(renderMask))
						continue;

					shader->setMeshParam(mesh, (*it_mesh)->m_entity);

					for (uint p = 0; p < passes; ++p)
					{
						shader->beginPass(p);

						sVBInfo* vbInfo = _getVBMgr()->get(mesh->m_vbBuid);
						sIBInfo* ibInfo = _getIBMgr()->get(mesh->m_ibBuid);

						cRenderer::renderIndexedPrimitive(vbInfo, ibInfo);

						shader->endPass();
					}
				}			
			}
		}

		shader->end();
	}
}