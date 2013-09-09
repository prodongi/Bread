/*
	BreadDecalAccumulate.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "BreadDecalAccumulate.h"
#include "BreadRenderData.h"

namespace Bread
{
	/*
	*/
	cDecalAccumulate::cDecalAccumulate()
	{
	}
	/*
	*/
	cDecalAccumulate::~cDecalAccumulate()
	{
	}
	/*
	*/
	void cDecalAccumulate::clear()
	{
	}
	/*
	*/
	void cDecalAccumulate::collect(cRenderData* renderData, cMaterial* mtl)
	{
		cMaterial::citer_mesh mtl_mesh = mtl->m_meshList.begin();
		for (; mtl_mesh != mtl->m_meshList.end(); ++mtl_mesh)
		{
			cMesh* mesh = renderData->getMesh(*mtl_mesh);
			if (!mesh)
				continue;

			m_list.pushBack(sDecalAccumulateData(mtl, mesh, renderData));
		}
	}
}