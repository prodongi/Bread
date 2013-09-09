
#ifndef _BreadRenderPropertyManager_h_
#define _BreadRenderPropertyManager_h_

#include "BreadDeclaration.h"

namespace Bread
{
	class cEngineConfig;

	/*
	 *	
	 */
	struct sRenderProperty
	{
		enum
		{
			ALPHABLENDENABLE,
			SRCBLEND,
			DESTBLEND,
			ZENABLE,
			ZWRITEENABLE,
			ZFUNC,
			LIGHTING,
			CULLMODE,
			FILLMODE,
			ANTIALIASEDLINEENABLE,
			SHADEMODE,
			ALPHATESTENABLE,
			ALPHAREF,
			ALPHAFUNC,
			COLOROP,
			COLORARG1,
			COLORARG2,
			ALPHAOP,
			ALPHAARG1,
			ALPHAARG2,
			MAGFILTER,
			MINFILTER,
			MIPFILTER,
			ADDRESSU,
			ADDRESSV,
			ADDRESSW,
			SCISSORTESTENABLE,
			BORDERCOLOR,
			DEPTHBIAS,
			SLOPESCALEDEPTHBIAS,
			TEXTURETRANSFORMFLAGS,
			TEXCOORDINDEX,
			DITHERENABLE,
			SPECULARENABLE,
			AMBIENT,
			NUM
		};
		sRenderProperty()
		{
			ZeroMemory(m_value, sizeof (DWORD) * NUM);
		}
		DWORD m_value[NUM];
	};
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
	 * \date 2010-02-07
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
	class cRenderPropertyManager
	{
	public:
		cRenderPropertyManager();
		~cRenderPropertyManager();
		void initDefault(cEngineConfig const* engineConfig);
		void setProperty(DWORD property, DWORD value);
		void begin();
		void end();
		void execOpSet(DWORD property, DWORD value);
		DWORD getDefaultValue(DWORD property) const	{	return m_default.m_value[property];	}

	private:
		typedef void (cRenderPropertyManager::*propertyOpSet)(DWORD);
		typedef void (cRenderPropertyManager::*propertyOpGet)(DWORD*);

		void applyDefault();
		void registPropertOp();
		void execOpGet(DWORD property, DWORD* value);

		void opSetAlphaBlendEnable(DWORD value);
		void opSetSrcBlend(DWORD value);
		void opSetDestBlend(DWORD value);
		void opSetZEnable(DWORD value);
		void opSetZWriteEnable(DWORD value);
		void opSetZFunc(DWORD value);
		void opSetLighting(DWORD value);
		void opSetCullMode(DWORD value);
		void opSetFillMode(DWORD value);
		void opSetAliasedLineEnable(DWORD value);
		void opSetShadeMode(DWORD value);
		void opSetAlphaTestEnable(DWORD value);
		void opSetAlphaRef(DWORD value);
		void opSetAlphaFunc(DWORD value);
		void opSetColorOp(DWORD value);
		void opSetColorArg1(DWORD value);
		void opSetColorArg2(DWORD value);
		void opSetAlphaOp(DWORD value);
		void opSetAlphaArg1(DWORD value);
		void opSetAlphaArg2(DWORD value);
		void opSetMagFilter(DWORD value);
		void opSetMinFilter(DWORD value);
		void opSetMipFilter(DWORD value);
		void opSetAddressU(DWORD value);
		void opSetAddressV(DWORD value);
		void opSetAddressW(DWORD value);
		void opSetScissorTestEnable(DWORD value);
		void opSetBorderColor(DWORD value);
		void opSetDepthBias(DWORD value);
		void opSetSlopeScaleDepthBias(DWORD value);
		void opSetTextureTransformFlags(DWORD value);
		void opSetTexcoordIndex(DWORD value);
		void opSetDitherEnable(DWORD value);
		void opSetSpecularEnable(DWORD value);
		void opSetAmbient(DWORD value);

		void opGetAlphaBlendEnable(DWORD* value);
		void opGetSrcBlend(DWORD* value);
		void opGetDestBlend(DWORD* value);
		void opGetZEnable(DWORD* value);
		void opGetZWriteEnable(DWORD* value);
		void opGetZFunc(DWORD* value);
		void opGetLighting(DWORD* value);
		void opGetCullMode(DWORD* value);
		void opGetFillMode(DWORD* value);
		void opGetAliasedLineEnable(DWORD* value);
		void opGetShadeMode(DWORD* value);
		void opGetAlphaTestEnable(DWORD* value);
		void opGetAlphaRef(DWORD* value);
		void opGetAlphaFunc(DWORD* value);
		void opGetColorOp(DWORD* value);
		void opGetColorArg1(DWORD* value);
		void opGetColorArg2(DWORD* value);
		void opGetAlphaOp(DWORD* value);
		void opGetAlphaArg1(DWORD* value);
		void opGetAlphaArg2(DWORD* value);
		void opGetMagFilter(DWORD* value);
		void opGetMinFilter(DWORD* value);
		void opGetMipFilter(DWORD* value);
		void opGetAddressU(DWORD* value);
		void opGetAddressV(DWORD* value);
		void opGetAddressW(DWORD* value);
		void opGetScissorTestEnable(DWORD* value);
		void opGetBorderColor(DWORD* value);
		void opGetDepthBias(DWORD* value);
		void opGetSlopeScaleDepthBias(DWORD* value);
		void opGetTextureTransformFlags(DWORD* value);
		void opGetTexcoordIndex(DWORD* value);
		void opGetDitherEnable(DWORD* value);
		void opGetSpecularEnable(DWORD* value);
		void opGetAmbient(DWORD* value);
	public:
		struct sProperty
		{
			sProperty(DWORD property, DWORD value, DWORD oldValue) : m_property(property), m_value(value), m_oldValue(oldValue) {}
			DWORD m_property;
			DWORD m_value;
			DWORD m_oldValue;
		};
	private:
		struct sCustomProperty
		{
			~sCustomProperty()	{	m_list.clear();	}
			std::vector<sProperty> m_list;
		};
		sRenderProperty m_default;
		std::vector<sCustomProperty> m_customList;
		propertyOpSet m_opSet[sRenderProperty::NUM];
		propertyOpGet m_opGet[sRenderProperty::NUM];
	};
}









#endif