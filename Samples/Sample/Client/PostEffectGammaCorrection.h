
#ifndef _PostEffectGammaCorrection_h_
#define _PostEffectGammaCorrection_h_


#include "BreadUiPrimitive.h"
#include "BreadRenderToTexture.h"
#include "Shader.h"

namespace Bread
{
	class cTexture;
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
 * \date 2010-03-18
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
 * \todo gpg4에서 참조, 상황에 맞게 밝기 조절 효과에 쓰면 좋음
 *
 * \bug 
 *
 */
class cPostEffectGammaCorrection
{
public:
	cPostEffectGammaCorrection();
	~cPostEffectGammaCorrection();
	bool initialize(uint width, uint height);
	void render(cTexture* originalTexture);
	void update(float elapsedtime);

private:
	bool initRtt(uint width, uint height);
	bool initShader();
	bool initTexture();
	/// @brief originalTexture에서 luminance값을 추출
	void renderLuminance(cTexture* originalTexture);
	/// @brief 감마 보정된 최종 화면 출력
	void renderCorrection(cTexture* originalTexture);
	void renderScreen();
	/// @brief histogram을 정규화 해서 min, max, average값을 구한다
	void buildNormalizedHistogram();
	/// @brief min, max, average값을 업데이트 해준다
	void updateGammaValue(float elapsedtime);
	void setGammaRamp();

	template <class T>
	void renderHistogram(float sx, float sy, float _scale, T* value)
	{
		LPDIRECT3DDEVICE9 device = cD3DSystem::getD3DDevice();
		device->SetTexture(0, NULL);
		device->SetFVF(sUiPlane::D3DFVF);

		int stride = sizeof (sUiVertex);
		sUiVertex bv[2];
		D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 0, 0);

		for (uint i = 0; i < 256; ++i)
		{
			float scale = min(100.0f, (float)value[i]*_scale);
			bv[0].set(D3DXVECTOR3((float)i+sx, sy, 0.0f), 0.0f, 0.0f, color);
			bv[1].set(D3DXVECTOR3((float)i+sx, sy - scale, 0.0f), 0.0f, 0.0f, color);
			cRenderer::renderPrimitiveUP(D3DPT_LINELIST, 1, (void*)bv, stride);
		}
	}

private:
	struct sHistogram
	{
		float m_lum[256];
		float m_min;
		float m_max;
		float m_avg;
	};

	cGammaCorrectionShader* m_shader;
	cRenderToTextureMSAA* m_rttLuminance;
	sUiPlane m_uiLuminance;
	sUiPlane m_uiCorrection;
	sUiPlane m_uiDebug;
	sHistogram m_histogram;
	float m_min, m_max, m_avg;
	float m_darkAdjustDt;
	float m_lightAdjustDt;
	float m_contrastAdjustDt;
	float m_bias;
	float m_darkClamp;
	float m_lightClamp;
	BYTE m_gammaRamp[256];
	cTexture* m_gammaRampTexture;
	uint m_gammaRampTexWidth, m_gammaRampTexHeight;
};











#endif