
#ifndef _BreadEngineConfig_h_
#define _BreadEngineConfig_h_

#include "BreadDeclaration.h"

namespace Bread
{
	/**
	 * \ingroup Core
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
	 * \date 2009-09-02
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
	class cEngineConfig
	{
	public:
		cEngineConfig();
		~cEngineConfig() {}

		bool load(std_string const& filename);

		TCHAR const* getTitle() const		{	return m_title.c_str();			}
		TCHAR const* getShadowType() const	{	return m_shadowType.c_str();	}
		TCHAR const* getWeatherTexture() const	{	return m_weatherTexture.c_str();	}
		propertyConstFunc(int, Width, m_width)
		propertyConstFunc(int, Height, m_height)
		propertyConstFunc(int, ColorDepth, m_colorDepth)
		propertyConstFunc(int, Frequency, m_frequency)
		propertyConstFunc(bool, FullScreen, m_fullScreen)
		propertyConstFunc(bool, VSync, m_vSync)
		propertyConstFunc(bool, Antialiasing, m_antialiasing)
		propertyConstFunc(bool, DepthBuffer, m_depthBuffer)
		propertyConstFunc(bool, UseWBuffer, m_useWBuffer)
		propertyConstFunc(bool, UseHUD, m_useHUD)
		propertyConstFunc(uint, MultiSampleType, m_multiSampleType)
		propertyConstFunc(uint, MultiSampleQuality, m_multiSampleQuality)
		propertyConstFunc(bool, RttMultiSampleFloat16, m_rttMultiSampleFloat16)
		propertyConstFunc(uint, RttMultiSampleType, m_rttMultiSampleType)
		propertyConstFunc(uint, RttMultiSampleQuality, m_rttMultiSampleQuality)
		propertyConstFunc(float, ZNear, m_zNear)
		propertyConstFunc(float, ZFar, m_zFar)
		propertyConstFunc(uint, ShadowWidth, m_shadowWidth)
		propertyConstFunc(uint, ShadowHeight, m_shadowHeight)
		propertyConstFunc(bool, FogEnable, m_fogEnable)
		propertyConstFunc(float, FogZNear, m_fogZNear)
		propertyConstFunc(float, FogZFar, m_fogZFar)
		propertyConstFunc(uint, FogColor, m_fogColor)
		propertyConstFunc(uint, WeatherParticleNum, m_weatherParticleNum)
		propertyConstFunc(float, WeatherParticleBoxSize, m_weatherParticleBoxSize)

	private:
		bool valueToBool(char const* value) const;

	private:
		std_string m_title;
		int m_width;
		int m_height;
		int m_colorDepth;
		int m_frequency;
		bool m_fullScreen;
		bool m_vSync;
		bool m_antialiasing;
		bool m_depthBuffer;
		bool m_useWBuffer;
		bool m_useHUD;
		uint m_multiSampleType;
		uint m_multiSampleQuality;
		/// @brief enable render to texture multi sample 16 
		bool m_rttMultiSampleFloat16;
		/// @brief enable render to texture multi sample type
		uint m_rttMultiSampleType;
		/// @brief enable render to texture multi sample quality
		uint m_rttMultiSampleQuality;
		float m_zNear;
		float m_zFar;
		/// @brief 그림자 타입
		std_string m_shadowType;
		uint m_shadowWidth;
		uint m_shadowHeight;
		bool m_fogEnable;
		float m_fogZNear;
		float m_fogZFar;
		uint m_fogColor;
		std_string m_weatherTexture;
		uint m_weatherParticleNum;
		float m_weatherParticleBoxSize;
	};
}














#endif