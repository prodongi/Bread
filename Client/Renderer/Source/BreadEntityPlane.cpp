
#include "BreadEntityPlane.h"
#include "BreadMeshCreater.h"
#include "BreadUberShaderManager.h"

namespace Bread
{
	implementRTTI(cEntityPlane, cEntityMesh)

	/*
	*/
	cEntityPlane::cEntityPlane()
	{
	}
	/*
	*/
	cEntityPlane::~cEntityPlane()
	{
	}
	/*
	*/
	bool cEntityPlane::createMesh(int width, int height, int segmentX, int segmentY, bool uvWrap, 
										std_string const& uberShaderType, std_string const& uberFragments, 
										std_string const& texFileName, bool isSceneObject, bool addOctree)
	{
		if (getBuid().empty())
		{
			assert(0 && _T("cEntityPlane::m_buid is empty"));
			return false;
		}

		float h_w = (float)(width>>1);
		float h_h = (float)(height>>1);

		sCullFigure cullFigure;
		cullFigure.m_box.set(-h_w, h_w, 0.0f, 1.0f, -h_h, h_h);
		cullFigure.m_sphere.m_radius = max(h_w, h_h);
		cullFigure.copyToBackup();

		cUberShader* shader = _getUberShaderMgr()->createShader(uberShaderType, uberFragments);
		createRenderData(&cullFigure, texFileName, shader->getBuid(), isSceneObject, addOctree);
		if (!_getMeshCreater()->createPlane(getBuid(), width, height, segmentX, segmentY, uvWrap))
		{
			return false;
		}

		return true;
	}
}