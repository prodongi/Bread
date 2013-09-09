
#include "stdafx.h"
#include "PostEffectEdgeId.h"
#include "BreadUberShaderManager.h"
#include "Shader.h"
#include "BreadSceneManager.h"
#include "BreadRenderPropertyManager.h"


/*
 *	
 */
cPostEffectEdgeId::cPostEffectEdgeId() : m_rtt(NULL)
{
}
/*
 *	
 */
cPostEffectEdgeId::~cPostEffectEdgeId()
{
	safeDelete(m_rtt);
}
/*
 *	
 */
bool cPostEffectEdgeId::initialize(uint width, uint height)
{
	assert(!m_rtt);
	m_rtt = new cRenderToTextureMSAA;
	if (!m_rtt->initialize(0, width, height))
		return false;

	return true;
}
/*
 *	
 */
void cPostEffectEdgeId::renderEdgeId()
{
	m_rtt->begin(true);
	_getSceneMgr()->renderSingleUberShader(_T("edgeid.fx"), _T("T0"));
	m_rtt->end();
}
/*
 *	
 */
void cPostEffectEdgeId::renderFinal()
{
	_getRenderPropertyMgr()->begin();
	_getRenderPropertyMgr()->setProperty(sRenderProperty::ALPHABLENDENABLE, TRUE);
	_getRenderPropertyMgr()->setProperty(sRenderProperty::SRCBLEND, D3DBLEND_ZERO);
	_getRenderPropertyMgr()->setProperty(sRenderProperty::DESTBLEND, D3DBLEND_SRCALPHA);
	_getRenderPropertyMgr()->setProperty(sRenderProperty::ZENABLE, FALSE);
	_getRenderPropertyMgr()->setProperty(sRenderProperty::LIGHTING, FALSE);
	_getRenderPropertyMgr()->setProperty(sRenderProperty::CULLMODE, D3DCULL_CCW);

	float w = (float)_getEngineConfig()->getWidth();
	float h = (float)_getEngineConfig()->getHeight();
	float du = 0.5f/w;
	float dv = 0.5f/h;
	sT4Vertex Vertex[4] = {
		//   x    y   z    rhw     tu       tv
		{ 0.0f,   0, 0.1f, 1.0f,  0.0f-du, 0.0f-dv // 좌측상단
								, 0.0f+du, 0.0f+dv // 우측하단
								, 0.0f-du, 0.0f+dv // 좌측하단
								, 0.0f+du, 0.0f-dv},//우측상단
		{    w,   0, 0.1f, 1.0f,  1.0f-du, 0.0f-dv
								, 1.0f+du, 0.0f+dv
								, 1.0f-du, 0.0f+dv
								, 1.0f+du, 0.0f-dv, },
		{    w,   h, 0.1f, 1.0f,  1.0f-du, 1.0f-dv
								, 1.0f+du, 1.0f+dv
								, 1.0f-du, 1.0f+dv
								, 1.0f+du, 1.0f-dv, },
		{ 0.0f,   h, 0.1f, 1.0f,  0.0f-du, 1.0f-dv
								, 0.0f+du, 1.0f+dv
								, 0.0f-du, 1.0f+dv
								, 0.0f+du, 1.0f-dv, },
	};

	cD3DSystem::getD3DDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX4 );

	cUberShader* shader = _getUberShaderMgr()->get(_T("edge.fx"));
	shader->setTechnique(_T("T0"));
	shader->setTexture(_T("baseTexture"), m_rtt->getTexture());

	uint pass;
	shader->begin(&pass);
	for (uint i = 0; i < pass; ++i)
	{
		shader->beginPass(i);
		cRenderer::renderPrimitiveUP(D3DPT_TRIANGLEFAN, 2, (void*)Vertex, sizeof (sT4Vertex));
		shader->endPass();
	}

	shader->end();

	_getRenderPropertyMgr()->end();
}