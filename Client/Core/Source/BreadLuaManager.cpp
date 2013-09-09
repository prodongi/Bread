
#include "BreadLuaManager.h"
#include "BreadCoreScript.h"
#include "BreadPostProcessManager.h"
#include "BreadAnimationActionEnum.h"

namespace Bread
{
	/*
	*/
	cLuaManager::cLuaManager()
	{
	}
	/*
	*/
	cLuaManager::~cLuaManager()
	{
	}
	/*
	*/
	void cLuaManager::init()
	{
		cLuaManagerBase::init();
		registerGlueFuncs();
		registerPredefineMembers();
		loadLuaFiles();
	}
	/*
	*/
	void cLuaManager::registerGlueFuncs()
	{
		registerLuaFunc_CoreScript();
	}
	/*
	*/
	void cLuaManager::registerPredefineMembers()
	{
		registerPredefinePostEffect();
		registerPredefineAnimaion();
		
	}
	/*
	*/
	void cLuaManager::registerPredefinePostEffect()
	{
		registerPredefineMember("POSTPROCESS_MONOCHROME",	cPostProcessManager::MONOCHROME);
		registerPredefineMember("POSTPROCESS_SEPIA",		cPostProcessManager::SEPIA);
		registerPredefineMember("POSTPROCESS_INVERSE",		cPostProcessManager::INVERSE);
		registerPredefineMember("POSTPROCESS_GBLUR_H",		cPostProcessManager::GBLUR_H);
		registerPredefineMember("POSTPROCESS_GBLUR_V",		cPostProcessManager::GBLUR_V);
		registerPredefineMember("POSTPROCESS_BLOOM_H",		cPostProcessManager::BLOOM_H);
		registerPredefineMember("POSTPROCESS_BLOOM_V",		cPostProcessManager::BLOOM_V);
		registerPredefineMember("POSTPROCESS_SCALE_ORI",	cPostProcessManager::SCALE_ORI);
		registerPredefineMember("POSTPROCESS_SCALE_HALF",	cPostProcessManager::SCALE_HALF);
		registerPredefineMember("POSTPROCESS_SCALE_QUART",	cPostProcessManager::SCALE_QUART);
		registerPredefineMember("POSTPROCESS_BRIGHT_PASS",	cPostProcessManager::BRIGHT_PASS);
		registerPredefineMember("POSTPROCESS_TONE_MAP",		cPostProcessManager::TONE_MAP);
		registerPredefineMember("POSTPROCESS_EDGE_DETECT",	cPostProcessManager::EDGE_DETECT);
		registerPredefineMember("POSTPROCESS_NORMAL_EDGE_DETECT",	cPostProcessManager::NORMAL_EDGE_DETECT);
		registerPredefineMember("POSTPROCESS_SSAO",			cPostProcessManager::SSAO);
		registerPredefineMember("POSTPROCESS_DOF",			cPostProcessManager::DOF);
	}
	/*
	*/
	void cLuaManager::registerPredefineAnimaion()
	{
		registerPredefineMember("ANI_IDLE",		ANI_IDLE);
		registerPredefineMember("ANI_WALK",		ANI_WALK);
		registerPredefineMember("ANI_RUN",		ANI_RUN);
		registerPredefineMember("ANI_ATTACK",	ANI_ATTACK);
		registerPredefineMember("ANI_DEAD",		ANI_DEAD);
	}
	/*
	*/
	void cLuaManager::loadLuaFiles()
	{
		doFile("script\\init.lua");
	}
}