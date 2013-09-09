
#include "BreadPostEffectBase.h"


namespace Bread
{
	/*
	 *	
	 */
	cPostEffectBase::cPostEffectBase() : m_shader(NULL), m_scaleType(-1)
	{
	}
	/*
	 *	
	 */
	cPostEffectBase::~cPostEffectBase()
	{
		safeDelete(m_shader);
	}
	/*
	 *	
	 */
	bool cPostEffectBase::initialize(uint effectType, uint width, uint height)
	{
		setEffectType(effectType);
		if (!initShader())
			return false;
		initShaderParam(width, height);
		initMember();
		return true;
	}
	/*
	 *	
	 */
	void cPostEffectBase::update(float elapsedtime)
	{
		unreferencedParameter(elapsedtime);
	}
}













