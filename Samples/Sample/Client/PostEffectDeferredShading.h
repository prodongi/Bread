
#ifndef _PostEffectDeferredShading_h_
#define _PostEffectDeferredShading_h_

#include "BreadUiPrimitive.h"
#include "BreadRenderToTexture.h"

namespace Bread
{
	class cLight;
	class cTexture;
}

class cDeferredLightManager
{
public:
	cDeferredLightManager() {}
	~cDeferredLightManager()		{	clear();				}
	void clear()					{	m_list.clear();			}
	uint size()	const				{	return m_list.size();	}
	cLight const* get(uint i) const	{	return m_list[i];		}
	void add(cLight const* light);

private:
	/// @brief 라이트 최대 개수
	static uint  MAX_LIGHT;
	/// @brief 라이트 간의 최소 거리
	static float MIN_SQRLEN;
	/// 카메라와 라이트간의 최대 거리
	static float MAX_CAM_SQRLEN;

	std::vector<cLight const*> m_list;
};

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
 * \date 2010-02-16
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
class cPostEffectDeferredShading
{
public:
	cPostEffectDeferredShading();
	~cPostEffectDeferredShading();
	bool initialize(uint width, uint height);
	void render(cTexture* originalTexture);
	void update(float elapsedtime);
	int renderDebugInfo(int y);
	sUiPlane const* getFinalScreen() const	{	return &m_screen;			}
	cTexture* getRttMaterialTexture()		{	return m_rttMaterial->getTexture();	}
	cTexture* getRttNormalTexture()			{	return m_rttNormal->getTexture();	}
	cTexture* getRttXYTexture()				{	return m_rttXY->getTexture();		}
	cTexture* getRttZTexture()				{	return m_rttZ->getTexture();		}

private:
	void setupMatrix();
	void renderBuild();
	void renderLight();

	void initLight();
	void updateLight(float elapsedtime);
	void updateDefLightMgr();

private:
	sUiPlane m_screen;
	float m_lightRotV;
	cRenderToTextureNAA* m_rttMaterial;
	cRenderToTextureNAA* m_rttNormal;
	cRenderToTextureNAA* m_rttXY;
	cRenderToTextureNAA* m_rttZ;
	D3DXMATRIXA16 m_matViewInv;
	cDeferredLightManager m_defLightMgr;
};













#endif