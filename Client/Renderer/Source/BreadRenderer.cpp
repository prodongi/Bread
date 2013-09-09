
#include <tchar.h>
#include "BreadRenderer.h"
#include "BreadRenderOutputInfo.h"
#include "BreadEngineConfig.h"
#include "BreadCameraManager.h"
#include "BreadSerialize3D.h"
#include "BreadSceneManager.h"
#include "BreadOctreeWorld.h"
#include "BreadOctreeNode.h"
#include "BreadRenderToTexture.h"
#include "BreadUiPrimitive.h"
#include "BreadLightManager.h"
#include "BreadShadowManager.h"
#include "BreadClassCreater.h"
#include "BreadRenderPrimitive.h"
#include "BreadEnvironment.h"
#include "BreadMsgInfo.h"
#include "BreadQuaternion.h"
#include "BreadAxis.h"
#include "BreadSkyBox.h"
#include "BreadProfile.h"
#include "BreadBatchRenderManager.h"
#include "BreadUberShaderManager.h"

namespace Bread
{
	/*
	 *	
	 */
	cRenderer::cRenderer() : m_d3dSystem(NULL), 
							 m_cameraMgr(NULL), 
							 m_sceneMgr(NULL), 
							 m_renderPropertyMgr(NULL),
							 m_lightMgr(NULL),
							 m_shadowMgr(NULL),
							 m_env(NULL),
							 m_axis(NULL)
	{
	}
	/*
	 *	
	 */
	cRenderer::~cRenderer()
	{
	}
	/*
	 *	
	 */
	bool cRenderer::initialize(HINSTANCE hInstance, WNDPROC wndProc, cEngineConfig const* config)
	{
		assert(!m_d3dSystem && _T("m_d3dSystem must be NULL"));
		assert(!m_cameraMgr && _T("m_cameraMgr must be NULL"));

		/*
		 *	d3dsystem
		 */
		m_d3dSystem = new cD3DSystem;
		m_d3dSystem->initialize(hInstance, wndProc, config);
		m_grid.initialize();

		//
		m_axis = new cAxis;
		m_axis->initialize();
		//
		m_cameraMgr = new cCameraManager;
		//
		m_sceneMgr = _getClassCreater()->newSceneMgr();
		m_sceneMgr->initialize();
		//
		m_renderPropertyMgr = new cRenderPropertyManager;
		m_renderPropertyMgr->initDefault(config);
		//
		m_lightMgr = _getClassCreater()->newLightMgr();
		//
		m_shadowMgr = _getClassCreater()->newShadowMgr();
		m_shadowMgr->initialize(config->getShadowType(), config->getShadowWidth(), config->getShadowHeight());
		//
		m_env = _getClassCreater()->newEnvironment();
		if (!m_env->initialize(cSkyBox::BOX, config))		
			return false;
		//
		cMaterial::createEmptyMtl();
		//
		_getUberShaderMgr()->createBaseShader();
		
		return true;
	}
	/*
	*/
	void cRenderer::finalize()
	{
		safeDelete(m_d3dSystem);
		safeDelete(m_cameraMgr);
		safeDelete(m_sceneMgr, true);
		safeDelete(m_renderPropertyMgr);
		safeDelete(m_lightMgr);
		safeDelete(m_shadowMgr);
		safeDelete(m_env, true);
		safeDelete(m_axis);
		//
		cSerialize3D::clearBuffer();
		cMaterial::deleteEmptyMtl();
	}
	/*
	 *	
	 */
	void cRenderer::beforeRender3D() 
	{
		m_shadowMgr->beforeRender3D();
	}
	void cRenderer::afterRender3D() 
	{
		m_shadowMgr->afterRender3D();
	}
	void cRenderer::beforeRender2D() 
	{
		m_shadowMgr->beforeRender2D();
	}
	void cRenderer::afterRender2D() 
	{
		m_shadowMgr->afterRender2D();
	}
	/*
	*/
	void cRenderer::msgProc(sMsgInfo* msg)
	{
		m_sceneMgr->msgProc(msg);
	}
	/*
	 *	
	 */
	bool cRenderer::renderPrimitive(D3DPRIMITIVETYPE primitiveType, unsigned int startVertex, 
									unsigned int primitiveCount, sVertexBuffer& vb, unsigned int stride)
	{
		if (cRenderOutputInfo::enable())
			cRenderOutputInfo::add(primitiveType, 0, primitiveCount);

		cD3DSystem::getD3DDevice()->SetStreamSource(0, vb.getStream(), 0, stride);
		HRESULT hr;
		if (FAILED(hr = cD3DSystem::getD3DDevice()->DrawPrimitive(primitiveType, startVertex, primitiveCount)))
		{
			assert(0 && _T("Failed render primitive"));
			return false;
		}
		return true;
	}
	/*
	 *	
	 */
	bool cRenderer::renderIndexedPrimitive(sVBInfo* vbInfo, sIBInfo* ibInfo)
	{
		if (!vbInfo || !ibInfo)
			return false;

		if (cRenderOutputInfo::enable())
			cRenderOutputInfo::add(vbInfo->m_primitiveType, vbInfo->m_vertexNum, ibInfo->m_faceNum);

		LPDIRECT3DDEVICE9 device = cD3DSystem::getD3DDevice();
		device->SetVertexDeclaration(vbInfo->m_decl);
		device->SetIndices(ibInfo->m_ib.getStream());
		device->SetStreamSource(0, vbInfo->m_vb.getStream(), 0, vbInfo->m_stride);

		HRESULT hr;
		if (FAILED(hr = device->DrawIndexedPrimitive(vbInfo->m_primitiveType, 0, 0, vbInfo->m_vertexNum, 0, ibInfo->m_faceNum)))
		{
			assert(0 && _T("Failed render indexed primitive"));
			return false;
		}
		return true;
	}
	/*
	 *	
	 */
	bool cRenderer::renderPrimitiveUP(D3DPRIMITIVETYPE primitiveType, unsigned int primitiveCount, 
									  const void* vertexStreamData, unsigned int stride)
	{
		if (cRenderOutputInfo::enable())
			cRenderOutputInfo::add(primitiveType, 0, primitiveCount);

		HRESULT hr;
		if (FAILED(hr = cD3DSystem::getD3DDevice()->DrawPrimitiveUP(primitiveType, primitiveCount, vertexStreamData, stride)))
		{
			assert(0 && _T("Failed render primitive up"));
			return false;
		}
		return true;
	}
	/*
	 *	
	 */
	bool cRenderer::renderIndexedPrimitiveUP(D3DPRIMITIVETYPE primitiveType, uint vertexNum, uint primitiveCount, 
											 const void* indexData, D3DFORMAT indexFormat, const void* vertexStreamData, uint stride)
	{
		if (cRenderOutputInfo::enable())
			cRenderOutputInfo::add(primitiveType, vertexNum, primitiveCount);

		HRESULT hr;
		if (FAILED(hr = cD3DSystem::getD3DDevice()->DrawIndexedPrimitiveUP(primitiveType, 0, vertexNum, primitiveCount, indexData, indexFormat, 
																		   vertexStreamData, stride)))
		{
			assert(0 && _T("Failed render index primitive up"));
			return false;
		}
		return true;
	}
	/*
	 *	
	 */
	void cRenderer::renderOctree(cOctreeWorld* worldOctree, int depth)
	{
		if (checkIsNull(worldOctree))
			return ;

		LPDIRECT3DDEVICE9 device = cD3DSystem::getD3DDevice();
		device->SetTexture(0, NULL);
		device->SetFVF(sLineVertex::D3DFVF);

		worldOctree->renderOctree(depth);
	}
	/*
	 *	
	 */
	void cRenderer::renderOctreeNode(cOctreeNode const* node, int depth)
	{
		if (!node)
			return ;

		if (node->getDepth() > depth)
			return ;

		renderBox(node->getBound(), cOctreeNode::m_renderColor); 

		for (int i = 0; i < cOctreeNode::REGION_NUM; ++i)
		{
			renderOctreeNode(node->getNode(i), depth);
		}
	}
	/*
	 *	
	 */
	void cRenderer::renderCullFigure(sCullFigure const* cullFigure, D3DCOLOR color)
	{
		// box
		renderBox(&cullFigure->m_box, color);
		// sphere
		renderSphere(&cullFigure->m_sphere, color);
	}
	/*
	 *	
	 */
	void cRenderer::renderBox(sBox const* box, DWORD color)
	{
		_getBatchRenderMgr()->push(box, color);
	}
	/*
	*/
	void cRenderer::renderSphere(sSphere const* sphere, DWORD color)
	{
		_getBatchRenderMgr()->push(sphere, color);
	}
	/*
	 *	
	 */
	void cRenderer::renderLine(sVector3 const& pos1, sVector3 const& pos2, DWORD color)
	{
		_getBatchRenderMgr()->push(&pos1, &pos2, color);
	}
	/*
	 *	
	 */
	void cRenderer::renderSingleEntity(cRenderToTextureBase* rtt, uint rttNum, cShader* shader,
									   std_string const& technique, cEntityModel const* entity, bool clear, D3DCOLOR clearColor)
	{
		uint i;
		for (i = 0; i < rttNum; ++i)
			rtt[i].begin(clear, clearColor);
		_getSceneMgr()->renderEntitySingleShader(shader, technique, entity);
		for (i = 0; i < rttNum; ++i)
			rtt[i].end();
	}
	/*
	 *
	 */
	void cRenderer::renderSingleScene(cRenderToTextureBase* rtt, uint rttNum, cShader* shader, 
									  std_string const& technique, bool clear, uint renderMask, D3DCOLOR clearColor)
	{
		uint i;
		for (i = 0; i < rttNum; ++i)
			rtt[i].begin(clear, clearColor);
		_getSceneMgr()->renderSingleShader(shader, technique, renderMask);
		for (i = 0; i < rttNum; ++i)
			rtt[i].end();
	}
	/*
	*/
	void cRenderer::renderRenderListScene(cRenderToTextureBase* rtt, uint rttNum, cShader* shader, std_string const& technique, 
										  bool clear, cBatchRenderManager::vecEntityList const& renderList, D3DCOLOR clearColor)
	{
		uint i;
		for (i = 0; i < rttNum; ++i)
			rtt[i].begin(clear, clearColor);
		_getSceneMgr()->renderRenderListShader(shader, technique, renderList);
		for (i = 0; i < rttNum; ++i)
			rtt[i].end();
	}
	/*
	 *	
	 */
	bool cRenderer::createVertexBuffer(uint size, DWORD fvf, void* buffer, sVertexBuffer& vb, uint usage, D3DPOOL pool, uint lock)
	{
		return vb.createStream(size, usage, fvf, pool, lock, buffer);
	}
	/*
	 *	
	 */
	bool cRenderer::createVertexBuffer(uint size, DWORD fvf, sVertexBuffer& vb, uint usage, D3DPOOL pool)
	{
		return vb.createStream(size, usage, fvf, pool, 0);
	}
	/*
	 *	
	 */
	bool cRenderer::createVertexBuffer(sVBInfo* info, void* buffer)
	{
		return cRenderer::createVertexBuffer(info->m_vbSize, 0, buffer, info->m_vb, info->m_usage, info->m_pool, info->m_lock);
	}
	/*
	*/
	bool cRenderer::createVertexBuffer(sVBInfo* info)
	{
		return cRenderer::createVertexBuffer(info->m_vbSize, 0, info->m_vb, info->m_usage, info->m_pool);
	}
	/*
	 *	
	 */
	bool cRenderer::createIndexBuffer(uint size, void* buffer, sIndexBuffer& ib, uint usage, D3DPOOL pool, uint lock)
	{
		return ib.createStream(size, usage, pool, lock, buffer);
	}
	/*
	*/
	bool cRenderer::createIndexBuffer(uint size, sIndexBuffer& ib, uint usage, D3DPOOL pool)
	{
		return ib.createStream(size, usage, pool, 0);
	}
	/*
	 *	
	 */
	bool cRenderer::createIndexBuffer(sIBInfo* info, void* buffer)
	{
		return cRenderer::createIndexBuffer(info->m_ibSize, buffer, info->m_ib, info->m_usage, info->m_pool, info->m_lock);
	}
	/*
	*/
	bool cRenderer::createIndexBuffer(sIBInfo* info)
	{
		return cRenderer::createIndexBuffer(info->m_ibSize, info->m_ib, info->m_usage, info->m_pool);
	}
	/*
	 *	
	 */
	void cRenderer::render()
	{
		if (_getIsRender(RENDER_WIRE))
		{
			_getRenderPropertyMgr()->begin();
			_getRenderPropertyMgr()->setProperty(sRenderProperty::FILLMODE, D3DFILL_WIREFRAME);
		}

		m_env->renderBeforeScene();
		m_axis->render();
		m_grid.render();
		m_sceneMgr->render();
		m_env->renderAfterScene();
		
		if (_getIsRender(RENDER_WIRE))
		{
			_getRenderPropertyMgr()->end();
		}
	}
	/*
	 *	
	 */
	void cRenderer::update(float elapsedtime)
	{
		cRenderOutputInfo::clear();

		{
			PROFILE(_T("env"));
			m_env->update(elapsedtime);
		}
		{
			PROFILE(_T("sceneMgr"));
			m_sceneMgr->update(elapsedtime);
		}
		{
			PROFILE(_T("cameraMgr"));
			m_cameraMgr->update(elapsedtime);
			m_cameraMgr->setTransform(cD3DSystem::getD3DDevice());
		}
		{
			PROFILE(_T("shadowMgr"));
			m_shadowMgr->update(elapsedtime);
		}
	}
	/*
	*/
	void cRenderer::mergeThread()
	{
		m_sceneMgr->mergeThread();
	}
	/*
	 *	
	 */
	LRESULT CALLBACK cRenderer::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (m_cameraMgr)
			m_cameraMgr->wndProc(hWnd, msg, wParam, lParam);

		return 1;
	}
	/*
	 *	
	 */
	int cRenderer::renderDebugInfo(int y)
	{
		y = m_sceneMgr->renderDebugInfo(y);
		y = m_shadowMgr->renderDebugInfo(y);
		y = m_cameraMgr->renderDebugInfo(y);
		y = m_lightMgr->renderDebugInfo(y);
		y = m_env->renderDebugInfo(y);
		return y;
	}
}


















