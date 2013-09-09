
#include "BreadRenderDataCollectorShader.h"
#include "BreadRenderDataManager.h"
#include "BreadAlphaAccumulate.h"
#include "BreadNoAlphaShaderAccumulate.h"
#include "BreadDecalAccumulate.h"
#include "BreadEntityModel.h"

namespace Bread
{
	/*
	 *	
	 */
	cRenderDataCollectorShader::cRenderDataCollectorShader()
	{
		m_alphaAccum = new cAlphaAccumulate;
		m_noalphaAccum = new cNoAlphaShaderAccumulate;
		m_decalAccum = new cDecalAccumulate;
	}
	/*
	 *	
	 */
	cRenderDataCollectorShader::~cRenderDataCollectorShader()
	{
		clearList();
		safeDelete(m_alphaAccum);
		safeDelete(m_noalphaAccum);
		safeDelete(m_decalAccum);
	}
	/*
	 *	
	 */
	void cRenderDataCollectorShader::clearList()
	{	
		m_alphaAccum->clear();
		m_noalphaAccum->clear();
		m_decalAccum->clear();
	}
	/*
	*/
	void cRenderDataCollectorShader::alphaSort()
	{
		m_alphaAccum->sort();
	}
	/*
	 *	
	 */
	void cRenderDataCollectorShader::collect(cRenderData* renderData)
	{
		if (!renderData)
			return ;

		//
		bool isDecal = renderData->m_entity->isDecal();

		cRenderData::bucket_mtl* mtlList = renderData->getMtlList();
		size_t mtlCount = mtlList->getBucketSize();
		// 맵이 안 입혀 졌을 때
		if (0 == mtlCount && !isDecal)
		{
			m_noalphaAccum->collectEmptyMtl(renderData, m_alphaAccum);
		}
		else
		{
			for (mtlList->begin(); !mtlList->isEnd(); mtlList->next())
			{
				cMaterial* mtl = mtlList->getIterData();

				if (isDecal)
				{
					m_decalAccum->collect(renderData, mtl);
				}
				else if (mtl->isAlpha())
				{
					m_alphaAccum->collect(renderData, mtl);
				}
				else
				{
					m_noalphaAccum->collect(renderData, mtl, m_alphaAccum);				
				}
			}
		}
	}
	/*
	*	
	*/
	void cRenderDataCollectorShader::renderCullFigure()
	{
		m_noalphaAccum->renderCullFigure();
		m_alphaAccum->renderCullFigure();
	}
	/*
	 *	
	 */
	void cRenderDataCollectorShader::update(float elapsedtime)
	{
		cRenderDataCollector::update(elapsedtime);

		m_noalphaAccum->update(elapsedtime);
		m_alphaAccum->update(elapsedtime);
	}
	/*
	*/
	void cRenderDataCollectorShader::render(cRenderDataManager const* /*rdMgr*/)
	{
		m_noalphaAccum->render();
		m_alphaAccum->render();
	}
	/*
	*/
	void cRenderDataCollectorShader::renderSingleShader(cShader* shader, std_string const& technique, uint renderMask)
	{
		if (!shader)
			return ;

		m_noalphaAccum->renderSingleShader(shader, technique, renderMask);
		m_alphaAccum->renderSingleShader(shader, technique, renderMask);
	}
}






















