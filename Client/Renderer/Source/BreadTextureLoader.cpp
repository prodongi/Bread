
#include "BreadTexture.h"
#include "BreadD3DSystemClassify.h"
#include "BreadUtils.h"
#include "BreadStringHelper.h"

namespace Bread
{
	/*
	 *	
	 */
	TCHAR const* cTextureLoader::makeBUID(std_string const& filename)
	{
		return _getStringHelper()->makeFilenameToBUID(filename);
	}

	/*
	 *	
	 */
	cTextureLoader::cTextureLoader()
	{
	}
	/*
	 *	
	 */
	cTextureLoader::~cTextureLoader()
	{		
	}
	/*
	 *	
	 */
	bool cTextureLoader::initialize(std_string const& filename, int textureType, int loadingType)
	{
		m_info = sTextureLoadInfo(cTextureLoader::makeBUID(filename), filename, textureType, loadingType);
		return true;
	}
	/*
	*/
	bool cTextureLoader::loadingCacheThread(sTextureLoadInfo const& info)
	{
		if (!initialize(info.getFileName(), info.m_textureType, info.m_loadingType))
			return false;
		if (!loadTexture(info.getFileName(), info.m_textureType))
			return false;

		return true;
	}
	/*	texture는 저장하지 않는다
	*/
	void cTextureLoader::setLoadingInfo(sTextureLoadInfo const& /*loadingInfo*/)
	{
	}
}