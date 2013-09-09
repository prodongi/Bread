
#include "BreadCubeTexture.h"
#include "BreadFileSystem.h"
#include "BreadD3DSystemClassify.h"

namespace Bread
{
	/*
	*/
	bool sCubeTexture::load(std_string const& filename)
	{
		std_string realPath;
		if (!_getFileSystem()->findRealPath(filename, realPath))
		{
			return false;
		}

		if (!checkHResultReturn(D3DXCreateCubeTextureFromFile(cD3DSystem::getD3DDevice(), realPath.c_str(), &m_tex)))
			return false;

		trace(_T("load texture : %s\n"), filename.c_str());

		return true;
	}
	/*
	*/
	void sCubeTexture::getDesc(D3DSURFACE_DESC& desc)
	{
		if (FAILED(m_tex->GetLevelDesc(0, &desc)))
		{
			assert(0 && "Failed texture get level desc");
		}
	}
}