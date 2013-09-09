
#ifndef _BreadShadowCSM_h_
#define _BreadShadowCSM_h_

#include "BreadMatrix4.h"
#include "BreadShadowBase.h"
#include "BreadRenderData.h"
#include "BreadFrustumPlane.h"
#include "BreadBatchRenderManager.h"

namespace Bread
{
	class cRenderToTextureBase;
	class cLightDepthMapShader;
	class cTexture;
	class cDepthTestShader;

	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	class cShadowCSM : public cShadowBase
	{
	protected:
		/*
		*/
		struct sRenderContext
		{
			sRenderContext() : m_rtt(NULL) {}
			~sRenderContext()	{	clear();	}
			bool initialize(uint width, uint height);
			void clear()
			{
				clearList();
				safeDelete(m_rtt);
			}
			void clearList()
			{
				m_renderList.clear();
			}

			sMatrix4 m_vp;
			float m_depth;
			cFrustumPlane m_frustum;
			cRenderToTextureBase* m_rtt;
			cBatchRenderManager::vecEntityList m_renderList;
		};
	public:
		cShadowCSM();
		virtual ~cShadowCSM();
		virtual bool initialize(uint width, uint height);
		virtual void update(float elapsedtime);
		virtual void beforeRender3D();
		virtual void afterRender3D();
		virtual void afterRender2D();
		virtual int renderDebugInfo(int y);
		cTexture* getDepthTexture(uint cascadedIndex) const;
		void getCascadedZNear(sVector4& znear);
		float getDepthBias() const							{	return m_depthBias;	}
		sMatrix4 const& getVP(uint cascadedIndex) const		{	return m_renderContext[cascadedIndex].m_vp;		}
		sMatrix4 const& getMatTex() const					{	return m_matTex;								}

		enum {	CASCADED_NUM = 4 };

		template <typename T> void extract(T* t)
		{
			bool isEntireVisible;
			for (uint i = 0; i < CASCADED_NUM; ++i)
			{
				m_renderContext[i].clearList();

				t->begin();
				for (; !t->isEnd(); t->next())
				{
					cRenderData* rd = t->getIterData().m_rd;
					if (!rd)
						continue;

					if (m_renderContext[i].m_frustum.isVisible(rd->m_cullFigure, isEntireVisible))
					{
						m_renderContext[i].m_renderList.push_back(cBatchRenderManager::sEntityList(rd));
					}
				}
			}
		}

	protected:
		bool initRenderContext(float zNear, float zFar);
		bool initDepthShader();
		void initMatTex();
		void calcSplitDepths(float zNear, float zFar, float splitLogFactor);
		void updateViewPort();
		void updateInput();
		/// @brief depthBias값을 카메라와의 거리에 따라서 자동으로 조절해 준다
		void updateBiasByCamLength();
		void calcRenderContextVP(int contextIndex);

	protected:
		sMatrix4 m_matTex;
		sRenderContext* m_renderContext;
		cLightDepthMapShader* m_depthShader;
		float m_depthBias;
		float m_lightDepthScale;
		float m_fovYDiv;
		float m_splitLogFactor;
	};

	//
	cShadowBase* createShadowCSM();
}



#endif