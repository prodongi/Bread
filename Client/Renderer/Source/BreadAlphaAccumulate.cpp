
#include "BreadAlphaAccumulate.h"
#include "BreadRenderData.h"
#include "BreadEntityModel.h"
#include "BreadCameraManager.h"
#include "BreadUberShaderManager.h"
#include "BreadRenderPropertyManager.h"
#include "BreadCore.h"

namespace Bread
{
	/*
	 *	
	 */
	void sAlphaAccumulateRender::operator () (sAlphaAccumulateData const& data)
	{
		cUberShader* shader = _getUberShaderMgr()->get(data.m_mtl->m_shaderBuid);
		if (!shader)
			shader = _getUberShaderMgr()->getBase();
		if (!shader)
			return ;

		shader->setMaterialParam(data.m_mtl);
		shader->setMeshParam(data.m_mesh, data.m_rd->m_entity);

		uint pass;
		shader->begin(&pass);
		for (uint p = 0; p < pass; ++p)
		{
			shader->beginPass(p);

			sVBInfo* vbInfo = _getVBMgr()->get(data.m_mesh->m_vbBuid);
			sIBInfo* ibInfo = _getIBMgr()->get(data.m_mesh->m_ibBuid);

			cRenderer::renderIndexedPrimitive(vbInfo, ibInfo);

			shader->endPass();
		}
		shader->end();
	}
	/*
	 *	
	 */
	void sAlphaAccumulateRenderCullFigure::operator () (sAlphaAccumulateData const& data)
	{
		cRenderer::renderCullFigure(&data.m_rd->m_cullFigure, 0xffff0000);
	}


	/*
	 *	
	 */
	cAlphaAccumulate::cAlphaAccumulate()
	{
		m_list.reserve(CONTAINER_SIZE);
	}
	/*
	 *	
	 */
	cAlphaAccumulate::~cAlphaAccumulate()
	{
		m_list.clear();

	}
	/*
	 *	
	 */
	void cAlphaAccumulate::collect(cRenderData* renderData, cMaterial const* mtl)
	{
		cMaterial::citer_mesh mtl_mesh = mtl->m_meshList.begin();
		for (; mtl_mesh != mtl->m_meshList.end(); ++mtl_mesh)
		{
			collectMesh(renderData, mtl, *mtl_mesh);
		}
	}
	/*
	*/
	void cAlphaAccumulate::collectMesh(cRenderData* renderData, cMaterial const* mtl, BUID const& meshBuid)
	{
		assert(renderData);
		assert(mtl);

		cMesh* mesh = renderData->getMesh(meshBuid);
		if (!mesh)
		{
			assert(!mesh);
			return ;
		}

		cCamera* camera = _getCameraMgr()->getMain();
		cEntityModel* entity = renderData->m_entity;

		sVector3 entityPos;
		addVec3(entity->getPosition(), mesh->m_cullFigure.m_sphere.m_center, entityPos);
		sVector3 vLen(camera->getEye());
		float len = vLen.lengthSquare(entityPos);

		m_list.push_back(sAlphaAccumulateData(len, mtl, mesh, renderData));
	}
	/*
	 *	
	 */
	void cAlphaAccumulate::sort()
	{
		std::sort(m_list.begin(), m_list.end(), sAlphaAccumulateOperator());
	}
	/*
	 *	
	 */
	void cAlphaAccumulate::render()
	{
		_getRenderPropertyMgr()->begin();
		_getRenderPropertyMgr()->setProperty(sRenderProperty::ALPHABLENDENABLE, TRUE);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::SRCBLEND, D3DBLEND_SRCALPHA);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::DESTBLEND, D3DBLEND_INVSRCALPHA);

		for_each(m_list.begin(), m_list.end(), sAlphaAccumulateRender());

		_getRenderPropertyMgr()->end();
	}
	/*
	*/
	void cAlphaAccumulate::renderCullFigure()
	{
		if (!_getIsRender(RENDER_MESH_CULLFIGURE))
			return ;

		_getRenderPropertyMgr()->begin();
		_getRenderPropertyMgr()->setProperty(sRenderProperty::ZENABLE, D3DZB_FALSE);

		for_each(m_list.begin(), m_list.end(), sAlphaAccumulateRenderCullFigure());

		_getRenderPropertyMgr()->end();
	}
	/*
	*/
	void cAlphaAccumulate::update(float elapsedtime)
	{
		std::vector<sAlphaAccumulateData>::iterator it = m_list.begin();
		for (; it < m_list.end(); ++it)
		{
			cUberShader* shader = _getUberShaderMgr()->get(it->m_mtl->m_shaderBuid);
			shader->update(elapsedtime, it->m_mesh, it->m_mtl, it->m_rd->m_entity);
		}
	}
	/*
	*/
	void cAlphaAccumulate::renderSingleShader(cShader* shader, std_string const& technique, uint renderMask)
	{
		_getRenderPropertyMgr()->begin();
		_getRenderPropertyMgr()->setProperty(sRenderProperty::ALPHABLENDENABLE, TRUE);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::SRCBLEND, D3DBLEND_SRCALPHA);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::DESTBLEND, D3DBLEND_INVSRCALPHA);

		uint passes;
		shader->setTechnique(technique.c_str());
		shader->setDefaultTexture();
		shader->begin(&passes);

		std::vector<sAlphaAccumulateData>::iterator it = m_list.begin();
		for (; it < m_list.end(); ++it)
		{
			if (!it->m_rd->m_entity->isValidRenderMask(renderMask))
				continue;

			shader->setMeshParam(it->m_mesh, it->m_rd->m_entity);

			for (uint p = 0; p < passes; ++p)
			{
				shader->beginPass(p);

				sVBInfo* vbInfo = _getVBMgr()->get(it->m_mesh->m_vbBuid);
				sIBInfo* ibInfo = _getIBMgr()->get(it->m_mesh->m_ibBuid);

				cRenderer::renderIndexedPrimitive(vbInfo, ibInfo);

				shader->endPass();
			}			
		}

		shader->end();	

		_getRenderPropertyMgr()->end();
	}
}