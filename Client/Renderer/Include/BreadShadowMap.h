
#ifndef _BreadShadowMap_h_
#define _BreadShadowMap_h_

#include "BreadShadowBase.h"
#include "BreadUiPrimitive.h"
#include "BreadRenderToTexture.h"
#include "BreadVector3.h"
#include "BreadVector4.h"
#include "BreadMatrix4.h"

namespace Bread
{
	class cShadowMapDepthShader;
	class cTexture;

	/**
	* \ingroup Sample
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
	* \date 2010-03-05
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
	* \todo PSM은 http://www.tulrich.com/geekstuff/psm/shadowbuffer.cpp에서 참조
	*
	* \bug 
	*
	*/
	class cShadowMap : public cShadowBase
	{
	public:
		cShadowMap();
		virtual ~cShadowMap();
		virtual bool initialize(uint width, uint height);
		virtual void update(float elapsedtime);
		virtual void beforeRender3D();
		virtual void afterRender2D();
		virtual int renderDebugInfo(int y);

		sMatrix4 const& getLightVP() const		{	return m_matLightVP;		}
		sMatrix4 const& getScaleBias() const	{	return m_matScaleBias;		}
		cTexture* getDepthTexture() const		{	return m_rttDepth->getTexture();	}

	protected:
		void initRtt();
		void initMatrix();
		bool initShader();
		void renderDepth();
		void buildOriginal();
		void buildPsm();
		sVector3 transformWorldToScreen(const sVector4& vec, sMatrix4 const& matViewProjPrime);
		void computeVirtualCameraParameters();

	protected:
		cRenderToTextureNAA* m_rttDepth;
		sMatrix4 m_matLightVP;
		sMatrix4 m_matScaleBias;
		sMatrix4 m_matLightProj;
		float m_zNear, m_zFar;
		float m_sbNear, m_sbFar;
		bool m_isPsm;
		float m_depthBias;
		cShadowMapDepthShader* m_shader;
	};

	//
	cShadowBase* createShadowMap();
}












#endif