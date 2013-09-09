
/*
	BreadPostEffectDecal.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "BreadPostEffectDecal.h"
#include "BreadPostEffectShaderList.h"
#include "BreadRenderer.h"

namespace Bread
{
	/*
	*/
	cPostEffectDecal::cPostEffectDecal()
	{
	}
	/*
	*/
	cPostEffectDecal::~cPostEffectDecal()
	{
	}
	/*
	*/
	bool cPostEffectDecal::initShader()
	{
		if (!createShader<cPostEffectDecalShader>(_T("posteffectdecal.fx")))
			return false;
		return true;
	}
	/*
	*/
	void cPostEffectDecal::render(cRenderToTextureBase* rtt, cTexture* preSceneTexture, sUiPlane const* screen)
	{
		cPostEffectBase::renderShader<cPostEffectDecalShader>(rtt, preSceneTexture, screen);
	}
}