
#include "BreadRenderPropertyManager.h"
#include "BreadD3DSystemClassify.h"
#include "BreadEngineConfig.h"


namespace Bread
{
	/*
	 *	
	 */
	struct sRenderPropertyFunctor
	{
		sRenderPropertyFunctor(cRenderPropertyManager* rpMgr) : m_rpMgr(rpMgr)	{}
		void operator ()(cRenderPropertyManager::sProperty const& property)
		{
			m_rpMgr->execOpSet(property.m_property, property.m_value);
		}
		cRenderPropertyManager* m_rpMgr;
	};
	/*
	 *  @brief property를 rpManager의 default값으로 복구
	 */
	struct sRenderPropertyRestoreFunctor
	{
		sRenderPropertyRestoreFunctor(cRenderPropertyManager* rpMgr) : m_rpMgr(rpMgr)	{}
		void operator ()(cRenderPropertyManager::sProperty const& property)
		{
			m_rpMgr->execOpSet(property.m_property, property.m_oldValue);
		}
		cRenderPropertyManager* m_rpMgr;
	};




	/*
	 *	
	 */
	cRenderPropertyManager::cRenderPropertyManager()
	{
		registPropertOp();
	}
	/*
	 *	
	 */
	cRenderPropertyManager::~cRenderPropertyManager()
	{
		m_customList.clear();
	}
	/*
	 *	
	 */
	void cRenderPropertyManager::initDefault(cEngineConfig const* engineConfig)
	{
		DWORD value;

		m_default.m_value[sRenderProperty::ALPHABLENDENABLE] = FALSE;
		m_default.m_value[sRenderProperty::SRCBLEND] = D3DBLEND_SRCALPHA;
		m_default.m_value[sRenderProperty::DESTBLEND] = D3DBLEND_INVSRCALPHA;
		value = (engineConfig->getUseWBuffer()) ? D3DZB_USEW : D3DZB_TRUE;
		m_default.m_value[sRenderProperty::ZENABLE] = value;
		m_default.m_value[sRenderProperty::ZWRITEENABLE] = TRUE;
		m_default.m_value[sRenderProperty::ZFUNC] = D3DCMP_LESSEQUAL;
		m_default.m_value[sRenderProperty::LIGHTING] = TRUE;
		m_default.m_value[sRenderProperty::CULLMODE] = D3DCULL_CCW;
		m_default.m_value[sRenderProperty::FILLMODE] = D3DFILL_SOLID;
		value = (engineConfig->getAntialiasing()) ? TRUE : FALSE;
		m_default.m_value[sRenderProperty::ANTIALIASEDLINEENABLE] = value;
		m_default.m_value[sRenderProperty::SHADEMODE] = D3DSHADE_GOURAUD;
		m_default.m_value[sRenderProperty::ALPHATESTENABLE] = FALSE;
		m_default.m_value[sRenderProperty::ALPHAREF] = 0x01;
		m_default.m_value[sRenderProperty::ALPHAFUNC] = D3DCMP_GREATEREQUAL;
		m_default.m_value[sRenderProperty::COLOROP] = D3DTOP_MODULATE;
		m_default.m_value[sRenderProperty::COLORARG1] = D3DTA_TEXTURE;
		m_default.m_value[sRenderProperty::COLORARG2] = D3DTA_DIFFUSE;
		m_default.m_value[sRenderProperty::ALPHAOP] = D3DTOP_MODULATE;
		m_default.m_value[sRenderProperty::ALPHAARG1] = D3DTA_TEXTURE;
		m_default.m_value[sRenderProperty::ALPHAARG2] = D3DTA_DIFFUSE;
		m_default.m_value[sRenderProperty::MAGFILTER] = D3DTEXF_LINEAR;
		m_default.m_value[sRenderProperty::MINFILTER] = D3DTEXF_LINEAR;
		m_default.m_value[sRenderProperty::MIPFILTER] = D3DTEXF_LINEAR;
		m_default.m_value[sRenderProperty::ADDRESSU] = D3DTADDRESS_WRAP;
		m_default.m_value[sRenderProperty::ADDRESSV] = D3DTADDRESS_WRAP;
		m_default.m_value[sRenderProperty::ADDRESSW] = D3DTADDRESS_WRAP;
		m_default.m_value[sRenderProperty::SCISSORTESTENABLE] = FALSE;
		m_default.m_value[sRenderProperty::BORDERCOLOR] = 0xffffffff;
		m_default.m_value[sRenderProperty::DEPTHBIAS] = 0;
		m_default.m_value[sRenderProperty::SLOPESCALEDEPTHBIAS] = 0;
		m_default.m_value[sRenderProperty::TEXTURETRANSFORMFLAGS] = D3DTTFF_DISABLE;
		m_default.m_value[sRenderProperty::TEXCOORDINDEX] = D3DTSS_TCI_PASSTHRU;
		m_default.m_value[sRenderProperty::DITHERENABLE] = FALSE;
		m_default.m_value[sRenderProperty::SPECULARENABLE] = FALSE;
		m_default.m_value[sRenderProperty::AMBIENT] = 0;

		applyDefault();
	}
	/*
	 *	
	 */
	void cRenderPropertyManager::registPropertOp()
	{
		m_opSet[sRenderProperty::ALPHABLENDENABLE] = &cRenderPropertyManager::opSetAlphaBlendEnable;
		m_opSet[sRenderProperty::SRCBLEND]	= &cRenderPropertyManager::opSetSrcBlend;
		m_opSet[sRenderProperty::DESTBLEND] = &cRenderPropertyManager::opSetDestBlend;
		m_opSet[sRenderProperty::ZENABLE]	= &cRenderPropertyManager::opSetZEnable;
		m_opSet[sRenderProperty::ZWRITEENABLE]	= &cRenderPropertyManager::opSetZWriteEnable;
		m_opSet[sRenderProperty::ZFUNC]		= &cRenderPropertyManager::opSetZFunc;
		m_opSet[sRenderProperty::LIGHTING]	= &cRenderPropertyManager::opSetLighting;
		m_opSet[sRenderProperty::CULLMODE]	= &cRenderPropertyManager::opSetCullMode;
		m_opSet[sRenderProperty::FILLMODE]	= &cRenderPropertyManager::opSetFillMode;
		m_opSet[sRenderProperty::ANTIALIASEDLINEENABLE]= &cRenderPropertyManager::opSetAliasedLineEnable;
		m_opSet[sRenderProperty::SHADEMODE] = &cRenderPropertyManager::opSetShadeMode;
		m_opSet[sRenderProperty::ALPHATESTENABLE] = &cRenderPropertyManager::opSetAlphaTestEnable;
		m_opSet[sRenderProperty::ALPHAREF]	= &cRenderPropertyManager::opSetAlphaRef;
		m_opSet[sRenderProperty::ALPHAFUNC] = &cRenderPropertyManager::opSetAlphaFunc;
		m_opSet[sRenderProperty::COLOROP]	= &cRenderPropertyManager::opSetColorOp;
		m_opSet[sRenderProperty::COLORARG1] = &cRenderPropertyManager::opSetColorArg1;
		m_opSet[sRenderProperty::COLORARG2] = &cRenderPropertyManager::opSetColorArg2;
		m_opSet[sRenderProperty::ALPHAOP] = &cRenderPropertyManager::opSetAlphaOp;
		m_opSet[sRenderProperty::ALPHAARG1] = &cRenderPropertyManager::opSetAlphaArg1;
		m_opSet[sRenderProperty::ALPHAARG2] = &cRenderPropertyManager::opSetAlphaArg2;
		m_opSet[sRenderProperty::MAGFILTER] = &cRenderPropertyManager::opSetMagFilter;
		m_opSet[sRenderProperty::MINFILTER] = &cRenderPropertyManager::opSetMinFilter;
		m_opSet[sRenderProperty::MIPFILTER] = &cRenderPropertyManager::opSetMipFilter;
		m_opSet[sRenderProperty::ADDRESSU]	= &cRenderPropertyManager::opSetAddressU;
		m_opSet[sRenderProperty::ADDRESSV]	= &cRenderPropertyManager::opSetAddressV;
		m_opSet[sRenderProperty::ADDRESSW]	= &cRenderPropertyManager::opSetAddressW;
		m_opSet[sRenderProperty::SCISSORTESTENABLE] = &cRenderPropertyManager::opSetScissorTestEnable;
		m_opSet[sRenderProperty::BORDERCOLOR] = &cRenderPropertyManager::opSetBorderColor;
		m_opSet[sRenderProperty::DEPTHBIAS] = &cRenderPropertyManager::opSetDepthBias;
		m_opSet[sRenderProperty::SLOPESCALEDEPTHBIAS] = &cRenderPropertyManager::opSetSlopeScaleDepthBias;
		m_opSet[sRenderProperty::TEXTURETRANSFORMFLAGS] = &cRenderPropertyManager::opSetTextureTransformFlags;
		m_opSet[sRenderProperty::TEXCOORDINDEX] = &cRenderPropertyManager::opSetTexcoordIndex;
		m_opSet[sRenderProperty::DITHERENABLE] = &cRenderPropertyManager::opSetDitherEnable;
		m_opSet[sRenderProperty::SPECULARENABLE] = &cRenderPropertyManager::opSetSpecularEnable;
		m_opSet[sRenderProperty::AMBIENT] = &cRenderPropertyManager::opSetAmbient;

		m_opGet[sRenderProperty::ALPHABLENDENABLE] = &cRenderPropertyManager::opGetAlphaBlendEnable;
		m_opGet[sRenderProperty::SRCBLEND]	= &cRenderPropertyManager::opGetSrcBlend;
		m_opGet[sRenderProperty::DESTBLEND] = &cRenderPropertyManager::opGetDestBlend;
		m_opGet[sRenderProperty::ZENABLE]	= &cRenderPropertyManager::opGetZEnable;
		m_opGet[sRenderProperty::ZWRITEENABLE]	= &cRenderPropertyManager::opGetZWriteEnable;
		m_opGet[sRenderProperty::ZFUNC]		= &cRenderPropertyManager::opGetZFunc;
		m_opGet[sRenderProperty::LIGHTING]	= &cRenderPropertyManager::opGetLighting;
		m_opGet[sRenderProperty::CULLMODE]	= &cRenderPropertyManager::opGetCullMode;
		m_opGet[sRenderProperty::FILLMODE]	= &cRenderPropertyManager::opGetFillMode;
		m_opGet[sRenderProperty::ANTIALIASEDLINEENABLE]= &cRenderPropertyManager::opGetAliasedLineEnable;
		m_opGet[sRenderProperty::SHADEMODE] = &cRenderPropertyManager::opGetShadeMode;
		m_opGet[sRenderProperty::ALPHATESTENABLE] = &cRenderPropertyManager::opGetAlphaTestEnable;
		m_opGet[sRenderProperty::ALPHAREF]	= &cRenderPropertyManager::opGetAlphaRef;
		m_opGet[sRenderProperty::ALPHAFUNC] = &cRenderPropertyManager::opGetAlphaFunc;
		m_opGet[sRenderProperty::COLOROP]	= &cRenderPropertyManager::opGetColorOp;
		m_opGet[sRenderProperty::COLORARG1] = &cRenderPropertyManager::opGetColorArg1;
		m_opGet[sRenderProperty::COLORARG2] = &cRenderPropertyManager::opGetColorArg2;
		m_opGet[sRenderProperty::ALPHAOP] = &cRenderPropertyManager::opGetAlphaOp;
		m_opGet[sRenderProperty::ALPHAARG1] = &cRenderPropertyManager::opGetAlphaArg1;
		m_opGet[sRenderProperty::ALPHAARG2] = &cRenderPropertyManager::opGetAlphaArg2;
		m_opGet[sRenderProperty::MAGFILTER] = &cRenderPropertyManager::opGetMagFilter;
		m_opGet[sRenderProperty::MINFILTER] = &cRenderPropertyManager::opGetMinFilter;
		m_opGet[sRenderProperty::MIPFILTER] = &cRenderPropertyManager::opGetMipFilter;
		m_opGet[sRenderProperty::ADDRESSU]	= &cRenderPropertyManager::opGetAddressU;
		m_opGet[sRenderProperty::ADDRESSV]	= &cRenderPropertyManager::opGetAddressV;
		m_opGet[sRenderProperty::ADDRESSW]	= &cRenderPropertyManager::opGetAddressW;
		m_opGet[sRenderProperty::SCISSORTESTENABLE] = &cRenderPropertyManager::opGetScissorTestEnable;
		m_opGet[sRenderProperty::BORDERCOLOR] = &cRenderPropertyManager::opGetBorderColor;
		m_opGet[sRenderProperty::DEPTHBIAS] = &cRenderPropertyManager::opGetDepthBias;
		m_opGet[sRenderProperty::SLOPESCALEDEPTHBIAS] = &cRenderPropertyManager::opGetSlopeScaleDepthBias;
		m_opGet[sRenderProperty::TEXTURETRANSFORMFLAGS] = &cRenderPropertyManager::opGetTextureTransformFlags;
		m_opGet[sRenderProperty::TEXCOORDINDEX] = &cRenderPropertyManager::opGetTexcoordIndex;
		m_opGet[sRenderProperty::DITHERENABLE] = &cRenderPropertyManager::opGetDitherEnable;
		m_opGet[sRenderProperty::SPECULARENABLE] = &cRenderPropertyManager::opGetSpecularEnable;
		m_opGet[sRenderProperty::AMBIENT] = &cRenderPropertyManager::opGetAmbient;
	}
	/*
	 *	
	 */
	void cRenderPropertyManager::setProperty(DWORD property, DWORD value)
	{
		if (m_default.m_value[property] != value)
		{
			DWORD oldValue;
			execOpGet(property, &oldValue);
			m_customList.back().m_list.push_back(sProperty(property, value, oldValue));
			execOpSet(property, value);
		}
	}
	/*
	 *	
	 */
	void cRenderPropertyManager::applyDefault()
	{
		for (int i = 0; i < sRenderProperty::NUM; ++i)
			execOpSet(i, m_default.m_value[i]);
	}
	/*
	 *	
	 */
	void cRenderPropertyManager::begin()
	{
		sCustomProperty cp;
		m_customList.push_back(cp);
	}
	/*
	 *	
	 */
	void cRenderPropertyManager::end()
	{
		std::for_each(m_customList.back().m_list.begin(), m_customList.back().m_list.end(), sRenderPropertyRestoreFunctor(this));
		m_customList.pop_back();
	}
	/*
	 *	
	 */
	void cRenderPropertyManager::execOpSet(DWORD property, DWORD value)
	{
		(this->*m_opSet[property])(value);
	}
	/*
	 *	
	 */
	void cRenderPropertyManager::execOpGet(DWORD property, DWORD* value)
	{
		(this->*m_opGet[property])(value);
	}
	/*
	 *	
	 */
	void cRenderPropertyManager::opSetAlphaBlendEnable(DWORD value)		{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, value);	}
	void cRenderPropertyManager::opSetSrcBlend(DWORD value)				{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_SRCBLEND, value);	}
	void cRenderPropertyManager::opSetDestBlend(DWORD value)			{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_DESTBLEND, value);	}
	void cRenderPropertyManager::opSetZEnable(DWORD value)				{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_ZENABLE, value);	}
	void cRenderPropertyManager::opSetZWriteEnable(DWORD value)			{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, value);	}
	void cRenderPropertyManager::opSetZFunc(DWORD value)				{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_ZFUNC, value);	}
	void cRenderPropertyManager::opSetLighting(DWORD value)				{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_LIGHTING, value);	}
	void cRenderPropertyManager::opSetCullMode(DWORD value	)			{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_CULLMODE, value);	}
	void cRenderPropertyManager::opSetFillMode(DWORD value	)			{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_FILLMODE, value);	}
	void cRenderPropertyManager::opSetAliasedLineEnable(DWORD value)	{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, value);	}
	void cRenderPropertyManager::opSetShadeMode(DWORD value)			{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_SHADEMODE, value);	}
	void cRenderPropertyManager::opSetAlphaTestEnable(DWORD value)		{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, value);	}
	void cRenderPropertyManager::opSetAlphaRef(DWORD value)				{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_ALPHAREF, value);	}
	void cRenderPropertyManager::opSetAlphaFunc(DWORD value)			{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_ALPHAFUNC, value);	}
	void cRenderPropertyManager::opSetColorOp(DWORD value)				{	cD3DSystem::getD3DDevice()->SetTextureStageState(0, D3DTSS_COLOROP, value);	}
	void cRenderPropertyManager::opSetColorArg1(DWORD value)			{	cD3DSystem::getD3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, value);	}
	void cRenderPropertyManager::opSetColorArg2(DWORD value)			{	cD3DSystem::getD3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, value);	}
	void cRenderPropertyManager::opSetAlphaOp(DWORD value)				{	cD3DSystem::getD3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, value);	}
	void cRenderPropertyManager::opSetAlphaArg1(DWORD value)			{	cD3DSystem::getD3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, value);	}
	void cRenderPropertyManager::opSetAlphaArg2(DWORD value)			{	cD3DSystem::getD3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, value);	}
	void cRenderPropertyManager::opSetMagFilter(DWORD value)			{	cD3DSystem::getD3DDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, value);	}
	void cRenderPropertyManager::opSetMinFilter(DWORD value)			{	cD3DSystem::getD3DDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, value);	}
	void cRenderPropertyManager::opSetMipFilter(DWORD value)			{	cD3DSystem::getD3DDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, value);	}
	void cRenderPropertyManager::opSetAddressU(DWORD value)				{	cD3DSystem::getD3DDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, value);	}
	void cRenderPropertyManager::opSetAddressV(DWORD value)				{	cD3DSystem::getD3DDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, value);	}
	void cRenderPropertyManager::opSetAddressW(DWORD value)				{	cD3DSystem::getD3DDevice()->SetSamplerState(0, D3DSAMP_ADDRESSW, value);	}
	void cRenderPropertyManager::opSetScissorTestEnable(DWORD value)	{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_SCISSORTESTENABLE, value);	}
	void cRenderPropertyManager::opSetBorderColor(DWORD value)			{	cD3DSystem::getD3DDevice()->SetSamplerState(0, D3DSAMP_BORDERCOLOR, value);	}
	void cRenderPropertyManager::opSetDepthBias(DWORD value)			{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_DEPTHBIAS, value);			}
	void cRenderPropertyManager::opSetSlopeScaleDepthBias(DWORD value)	{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, value);	}
	void cRenderPropertyManager::opSetTextureTransformFlags(DWORD value){	cD3DSystem::getD3DDevice()->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, value);	}
	void cRenderPropertyManager::opSetTexcoordIndex(DWORD value)		{	cD3DSystem::getD3DDevice()->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, value);	}
	void cRenderPropertyManager::opSetDitherEnable(DWORD value)			{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_DITHERENABLE, value);	}
	void cRenderPropertyManager::opSetSpecularEnable(DWORD value)		{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_SPECULARENABLE, value);	}
	void cRenderPropertyManager::opSetAmbient(DWORD value)				{	cD3DSystem::getD3DDevice()->SetRenderState(D3DRS_AMBIENT, value);	}
	/*
	 *	
	 */
	void cRenderPropertyManager::opGetAlphaBlendEnable(DWORD* value)	{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_ALPHABLENDENABLE, value);	}
	void cRenderPropertyManager::opGetSrcBlend(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_SRCBLEND, value);	}
	void cRenderPropertyManager::opGetDestBlend(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_DESTBLEND, value);	}
	void cRenderPropertyManager::opGetZEnable(DWORD* value)				{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_ZENABLE, value);	}
	void cRenderPropertyManager::opGetZWriteEnable(DWORD* value)		{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_ZWRITEENABLE, value);	}
	void cRenderPropertyManager::opGetZFunc(DWORD* value)				{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_ZFUNC, value);	}
	void cRenderPropertyManager::opGetLighting(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_LIGHTING, value);	}
	void cRenderPropertyManager::opGetCullMode(DWORD* value	)			{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_CULLMODE, value);	}
	void cRenderPropertyManager::opGetFillMode(DWORD* value	)			{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_FILLMODE, value);	}
	void cRenderPropertyManager::opGetAliasedLineEnable(DWORD* value)	{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_ANTIALIASEDLINEENABLE, value);	}
	void cRenderPropertyManager::opGetShadeMode(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_SHADEMODE, value);	}
	void cRenderPropertyManager::opGetAlphaTestEnable(DWORD* value)		{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_ALPHATESTENABLE, value);	}
	void cRenderPropertyManager::opGetAlphaRef(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_ALPHAREF, value);	}
	void cRenderPropertyManager::opGetAlphaFunc(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_ALPHAFUNC, value);	}
	void cRenderPropertyManager::opGetColorOp(DWORD* value)				{	cD3DSystem::getD3DDevice()->GetTextureStageState(0, D3DTSS_COLOROP, value);	}
	void cRenderPropertyManager::opGetColorArg1(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetTextureStageState(0, D3DTSS_COLORARG1, value);	}
	void cRenderPropertyManager::opGetColorArg2(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetTextureStageState(0, D3DTSS_COLORARG2, value);	}
	void cRenderPropertyManager::opGetAlphaOp(DWORD* value)				{	cD3DSystem::getD3DDevice()->GetTextureStageState(0, D3DTSS_ALPHAOP, value);	}
	void cRenderPropertyManager::opGetAlphaArg1(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetTextureStageState(0, D3DTSS_ALPHAARG1, value);	}
	void cRenderPropertyManager::opGetAlphaArg2(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetTextureStageState(0, D3DTSS_ALPHAARG2, value);	}
	void cRenderPropertyManager::opGetMagFilter(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetSamplerState(0, D3DSAMP_MAGFILTER, value);	}
	void cRenderPropertyManager::opGetMinFilter(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetSamplerState(0, D3DSAMP_MINFILTER, value);	}
	void cRenderPropertyManager::opGetMipFilter(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetSamplerState(0, D3DSAMP_MIPFILTER, value);	}
	void cRenderPropertyManager::opGetAddressU(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetSamplerState(0, D3DSAMP_ADDRESSU, value);	}
	void cRenderPropertyManager::opGetAddressV(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetSamplerState(0, D3DSAMP_ADDRESSV, value);	}
	void cRenderPropertyManager::opGetAddressW(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetSamplerState(0, D3DSAMP_ADDRESSW, value);	}
	void cRenderPropertyManager::opGetScissorTestEnable(DWORD* value)	{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_SCISSORTESTENABLE, value);	}
	void cRenderPropertyManager::opGetBorderColor(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetSamplerState(0, D3DSAMP_BORDERCOLOR, value);	}
	void cRenderPropertyManager::opGetDepthBias(DWORD* value)			{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_DEPTHBIAS, value);			}
	void cRenderPropertyManager::opGetSlopeScaleDepthBias(DWORD* value)	{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, value);	}
	void cRenderPropertyManager::opGetTextureTransformFlags(DWORD* value){	cD3DSystem::getD3DDevice()->GetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, value);	}
	void cRenderPropertyManager::opGetTexcoordIndex(DWORD* value)		{	cD3DSystem::getD3DDevice()->GetTextureStageState(0, D3DTSS_TEXCOORDINDEX, value);	}
	void cRenderPropertyManager::opGetDitherEnable(DWORD* value)		{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_DITHERENABLE, value);	}
	void cRenderPropertyManager::opGetSpecularEnable(DWORD* value)		{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_SPECULARENABLE, value);	}
	void cRenderPropertyManager::opGetAmbient(DWORD* value)				{	cD3DSystem::getD3DDevice()->GetRenderState(D3DRS_AMBIENT, value);	}
}