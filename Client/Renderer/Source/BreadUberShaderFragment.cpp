
#include "BreadUberShaderFragment.h"
#include "BreadStringHelper.h"
#include "BreadShadowManager.h"
#include "BreadEnvironment.h"
#include "BreadFog.h"
#include "BreadCore.h"

namespace Bread
{
	implementInstance(cUberShaderFragment)

	/*
	*/
	cUberShaderFragment::cUberShaderFragment()
	{
		assignInstance(this)
	}
	/*
	*/
	cUberShaderFragment::~cUberShaderFragment()
	{
		assignInstance(NULL)
		m_list.clear();
	}
	/*
	*/
	void cUberShaderFragment::initialize()
	{
		addFragments();
	}
	/*
	*/
	void cUberShaderFragment::addFragments()
	{
		m_list.reserve(FRAGMENT_NUM);

		m_list.push_back(sFragment(0x00, FRAGMENT_NONE, ""));
		m_list.push_back(sFragment(0x01, FRAGMENT_BASE, "#define _BASE_\r\n"));
		m_list.push_back(sFragment(0x02, FRAGMENT_SPECULAR, "#define _SPECULAR_\r\n"));
		m_list.push_back(sFragment(0x04, FRAGMENT_BUMP, "#define _BUMP_\r\n"));
		m_list.push_back(sFragment(0x08, FRAGMENT_DEFERRED, "#define _DEFERRED_\r\n"));
		m_list.push_back(sFragment(0x10, FRAGMENT_GPU_ANI, "#define _GPU_ANI_\r\n"));
		m_list.push_back(sFragment(0x20, FRAGMENT_POM, "#define _POM_\r\n"));
		m_list.push_back(sFragment(0x40, FRAGMENT_VISUAL_NORMAL, "#define _VISUAL_NORMAL_\r\n"));
		m_list.push_back(sFragment(0x80,FRAGMENT_VEGETATION_ANI, "#define _VEGETATION_ANI_\r\n"));
	}
	/*
	*/
	uint cUberShaderFragment::stringToFragment(TCHAR const* str)
	{
		std::vector<std_string> strList;
		_getStringHelper()->split(str, _T("|"), strList);

		uint fragments = 0;
		size_t fragmentNum = m_list.size();
		size_t len = strList.size();
		for (size_t i = 0; i < len; ++i)
		{
			for (size_t f = 0; f < fragmentNum; ++f)
			{
				if (strList[i] == m_list[f].m_token)
				{
					fragments += m_list[f].m_flag;
					break;
				}
			}
		}
		strList.clear();
		return fragments;
	}
	/*
	*/
	void cUberShaderFragment::fragmentToDefinition(uint fragments, std::string& effectData)
	{
		size_t fragmentNum = m_list.size();
		for (size_t f = 0; f < fragmentNum; ++f)
		{
			if (fragments & m_list[f].m_flag)	effectData += m_list[f].m_definition;
		}
	}
	/*
	*/
	void cUberShaderFragment::addDefinitionFragments(std::string const& srcEffectData, uint fragments, std::string& effectData)
	{
		fragmentToDefinition(fragments, effectData);

		// shadow
		if (_getShadowMgr()->isType(_T("projected")))			effectData += "#define _PROJECTEDSHADOW_\r\n";
		else if (_getShadowMgr()->isType(_T("shadowmap")))		effectData += "#define _SHADOWMAP_\r\n";
		else if (_getShadowMgr()->isType(_T("shadowvolume")))	effectData += "#define _SHADOWVOLUME_\r\n";
		else if (_getShadowMgr()->isType(_T("csm")))			effectData += "#define _CSM_\r\n";

		// fog
		if (_getEnv()->getFog()->getEnable())
			effectData += "#define _FOG_\r\n";

		effectData += srcEffectData;
	}
	/*
	*/
	bool cUberShaderFragment::is(uint fragment, uint fragments)
	{
		if (fragment >= m_list.size())
		{
			assert(0 && _T("invalid fragment value"));
			return false;
		}
		return (m_list[fragment].m_flag & fragments) ? true : false;
	}
}