
#ifndef _BreadProjectedShadow_h_
#define _BreadProjectedShadow_h_

#include "BreadShadowBase.h"
#include "BreadRenderToTexture.h"
#include "BreadUiPrimitive.h"
#include "BreadMatrix4.h"

namespace Bread
{
	class cProjectedShadowShader;
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
	* \todo 그림자가 필요한 모든 오브젝트를 한 텍스춰에 그린다
	*
	* \bug 
	*
	*/
	class cProjectedShadow : public cShadowBase
	{
	public:
		cProjectedShadow();
		virtual ~cProjectedShadow();
		virtual bool initialize(uint width, uint height);
		virtual void update(float elapsedtime);
		virtual void beforeRender3D();
		virtual void afterRender2D();

		sMatrix4 const& getMatShadowVP()	const	{	return m_matShadowVP;	}
		sMatrix4 const& getMatShadowTex()	const	{	return m_matShadowTex;	}
		cTexture* getShadowTexture() const			{	return m_rttShadow->getTexture();	}

	private:
		void initRtt();
		void initMatrix();
		bool initShader();
		void makeShadowMap();
		void renderDebug();
		void updateMatrix();

	private:
		cRenderToTextureNAA* m_rttShadow;
		sMatrix4 m_matShadowView;
		sMatrix4 m_matShadowProj;
		sMatrix4 m_matShadowVP;
		sMatrix4 m_matShadowTex;

		cProjectedShadowShader* m_shader;
	};

	//
	cShadowBase* createShadowProjected();
}









#endif