
#ifndef _BreadRenderer_h_
#define _BreadRenderer_h_

#include "BreadCore.h"
#include "BreadD3DSystemClassify.h"
#include "BreadGrid.h"
#include "BreadVBManager.h"
#include "BreadIBManager.h"
#include "BreadRenderPropertyManager.h"
#include "BreadShader.h"
#include "BreadTexture.h"
#include "BreadBatchRenderManager.h"

namespace Bread
{
	class cEngineConfig;
	class cCameraManager;
	class cSceneManager;
	class cOctreeWorld;
	class cOctreeNode;
	class cRenderToTextureBase;
	class cLightManager;
	class cShadowManager;
	class cShader;
	class cEntityModel;
	class cAxis;
	class cEnvironment;
	struct sCullFigure;
	struct sBox;
	struct sUiPlane;
	struct sMsgInfo;

	/**
	 * \ingroup Renderer
	 *
	 *
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2009-08-20
	 *
	 * \author Prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	class cRenderer
	{
		declareClassCreater(cRenderer)

	public:
		virtual bool initialize(HINSTANCE hInstance, WNDPROC wndProc, cEngineConfig const* config);
		virtual void finalize();
		virtual void beforeRender3D();
		virtual void afterRender3D();
		virtual void beforeRender2D();
		virtual void afterRender2D();
		virtual void msgProc(sMsgInfo* /*msg*/);

		cD3DSystem* getD3DSystem() const					{	return m_d3dSystem;			}
		cCameraManager* getCameraMgr() const				{	return m_cameraMgr;			}
		cSceneManager* getSceneMgr() const					{	return m_sceneMgr;			}
		cRenderPropertyManager* getRenderPropertyMgr() const{	return m_renderPropertyMgr;	}
		cShadowManager* getShadowMgr() const				{	return m_shadowMgr;			}
		virtual void render();
		virtual void update(float elapsedtime);
		virtual void mergeThread();
		virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
		virtual int renderDebugInfo(int y);

		static bool renderPrimitive(D3DPRIMITIVETYPE primitiveType, unsigned int startVertex, unsigned int primitiveCount,
									   sVertexBuffer& vb, unsigned int stride);
		static bool renderIndexedPrimitive(sVBInfo* vbInfo, sIBInfo* ibInfo);
		static bool renderPrimitiveUP(D3DPRIMITIVETYPE primitiveType, unsigned int primitiveCount, 
										const void* vertexStreamData, unsigned int stride);
		static bool renderIndexedPrimitiveUP(D3DPRIMITIVETYPE primitiveType, uint vertexNum, uint primitiveCount, 
										const void* indexData, D3DFORMAT indexFormat, const void* vertexStreamData, uint stride);
		static void renderOctree(cOctreeWorld* worldOctree, int depth);
		static void renderCullFigure(sCullFigure const* cullFigure, DWORD color);
		static void renderBox(sBox const* box, DWORD color);
		static void renderSphere(sSphere const* sphere, DWORD color);
		static void renderLine(sVector3 const& pos1, sVector3 const& pos2, DWORD color);
		static void renderSingleEntity(cRenderToTextureBase* rtt, uint rttNum, cShader* shader, std_string const& technique, cEntityModel const* entity, bool clear, D3DCOLOR clearColor = 0);
		static void renderSingleScene(cRenderToTextureBase* rtt, uint rttNum, cShader* shader, std_string const& technique, bool clear, uint renderMask = 0, D3DCOLOR clearColor = 0);
		static void renderRenderListScene(cRenderToTextureBase* rtt, uint rttNum, cShader* shader, std_string const& technique, bool clear, cBatchRenderManager::vecEntityList const& renderList, D3DCOLOR clearColor = 0);
		static bool createVertexBuffer(uint size, DWORD fvf, void* buffer, sVertexBuffer& vb, 
										  uint usage = D3DUSAGE_WRITEONLY, D3DPOOL pool = D3DPOOL_MANAGED, uint lock = 0);
		static bool createVertexBuffer(uint size, DWORD fvf, sVertexBuffer& vb, uint usage = D3DUSAGE_WRITEONLY, D3DPOOL pool = D3DPOOL_MANAGED);
		static bool createVertexBuffer(sVBInfo* info, void* buffer);
		static bool createVertexBuffer(sVBInfo* info);
		static bool createIndexBuffer(uint size, void* buffer, sIndexBuffer& ib, 
										 uint usage = D3DUSAGE_WRITEONLY, D3DPOOL pool = D3DPOOL_MANAGED, uint lock = 0);
		static bool createIndexBuffer(uint size, sIndexBuffer& ib, uint usage = D3DUSAGE_WRITEONLY, D3DPOOL pool = D3DPOOL_MANAGED);
		static bool createIndexBuffer(sIBInfo* info, void* buffer);
		static bool createIndexBuffer(sIBInfo* info);

		template <class T>
			static void renderScreen(cRenderToTextureBase* rtt, cShader* shader, T const* screen, bool clear = false)
		{
			_getRenderPropertyMgr()->begin();
			_getRenderPropertyMgr()->setProperty(sRenderProperty::CULLMODE, D3DCULL_CCW);

			rtt->begin(clear);
			renderScreen(shader, screen);
			rtt->end();

			_getRenderPropertyMgr()->end();
		}
		template <class T>
			static void renderScreen(cRenderToTextureBase* rtt, T const* screen, cTexture* texture, bool clear = false)
		{
			_getRenderPropertyMgr()->begin();
			_getRenderPropertyMgr()->setProperty(sRenderProperty::CULLMODE, D3DCULL_CCW);

			rtt->begin(clear);
			renderScreen(screen, texture);
			rtt->end();

			_getRenderPropertyMgr()->end();
		}
		template <class T>
			static void renderScreen(cShader* shader, T const* screen)
		{
			cD3DSystem::getD3DDevice()->SetFVF(T::D3DFVF);
			uint pass;
			shader->begin(&pass);
			for (uint p = 0; p < pass; ++p)
			{
				shader->beginPass(p);
				cRenderer::renderPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (void const*)screen->m_vertex, T::VERTEX_SIZE);
				shader->endPass();
			}
			shader->end();
		}
		template <class T>
			static void renderScreen(T const* screen, cTexture* texture)
		{
			cD3DSystem::getD3DDevice()->SetTexture(0, texture->getTexture());
			renderScreen(screen);
		}
		template <class T>
			static void renderScreen(T const* screen)
		{
			_getRenderPropertyMgr()->begin();
			_getRenderPropertyMgr()->setProperty(sRenderProperty::CULLMODE, D3DCULL_CCW);

			LPDIRECT3DDEVICE9 device = cD3DSystem::getD3DDevice();
			device->SetFVF(T::D3DFVF);
			cRenderer::renderPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (void const*)screen->m_vertex, T::VERTEX_SIZE);

			_getRenderPropertyMgr()->end();
		}
		template <class T>
			static void renderDebugScreen(float sx, float sy, float w, float h, cTexture* texture)
		{
			static T t;
			t.set(sx, sy, w, h);
			cRenderer::renderScreen(&t, texture);
		}

		static void renderOctreeNode(cOctreeNode const* node, int depth);

	protected:
		cD3DSystem*		m_d3dSystem;
		cCameraManager* m_cameraMgr;
		cLightManager*	m_lightMgr;
		cSceneManager*	m_sceneMgr;
		cRenderPropertyManager* m_renderPropertyMgr;
		cShadowManager* m_shadowMgr;
		cAxis*			m_axis;
		cGrid			m_grid;
		cEnvironment* m_env;
	};
}






#endif