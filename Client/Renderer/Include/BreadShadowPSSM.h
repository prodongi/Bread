
#ifndef _BreadShadowPSSM_h_
#define _BreadShadowPSSM_h_

#include "BreadShadowBase.h"
#include "BreadUiPrimitive.h"
#include "BreadRenderToTexture.h"
#include "BreadRenderData.h"

namespace Bread
{
	class cShadowPSSMShader;

	/*
		@date 2010.07.26
		@auth prodongi
		@desc 
		@todo 인터넷에 공개된 소스 참조
	*/
	class cShadowPSSM : public cShadowBase
	{
	public:
		cShadowPSSM();
		virtual ~cShadowPSSM();
		virtual bool initialize(uint width, uint height);
		virtual void update(float elapsedtime);
		virtual void beforeRender3D();
		virtual void afterRender2D();
		virtual int renderDebugInfo(int y);

		sMatrix4 const& getLightVP() const	{	return m_matLightVP;	}

	private:
		bool initRtt();
		bool initShader();
		void initEtc();
		void updateNearFar();
		void updateSplitDistance();

	public:
		static int splitNum;
		static float splitScemeLambda;
	protected:
		struct sSplitInfo
		{
			sSplitInfo() : m_rtt(NULL), m_distance(0.0f) {}
			~sSplitInfo()	{	safeDelete(m_rtt);	}
			cRenderToTextureNAA* m_rtt;
			float m_distance;
			sMatrix4 m_matLightVP;
		};

		sSplitInfo* m_splitInfo;
		sBox m_sceneAABB;	// scene의 가시영역에 관한 AABB(여기서는 임시로 사용)
		float m_camNear;
		float m_camFar;
		cShadowPSSMShader* m_shader;
		sMatrix4	m_matLightVP;
	};

	//
	cShadowBase* createShadowPSSM();
}

#endif
