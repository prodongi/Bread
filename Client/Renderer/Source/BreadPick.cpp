
#include "BreadPick.h"
#include "BreadCamera.h"
#include "BreadOctreeNode.h"
#include "BreadCore.h"
#include "BreadTerrain.h"
#include "BreadUtils.h"
#include "BreadRenderPropertyManager.h"
#include "BreadStringHelper.h"
#include "BreadGui.h"

namespace Bread
{
	/*
	 *	
	 */
	struct sRenderOctreeNode
	{
		void operator ()(cOctreeNode const* node)
		{
			cRenderer::renderBox(node->getBound(), D3DCOLOR_ARGB(255, 0, 255, 0));
		}
	};
	/*
	 *	
	 */
	struct sRenderRdList
	{
		sRenderRdList(cTerrain const* terrain)
		{
			m_terrain = terrain;
		}
		void operator ()(BUID const& buid)
		{
			char temp[NAME_LEN];
			_getStringHelper()->w2m(buid.c_str(), temp, NAME_LEN);
			uint const* vindex = m_terrain->getVertIndex(atoi(temp));

			cRenderer::renderLine(m_terrain->getVector(vindex[0]), m_terrain->getVector(vindex[1]), 0xff0000ff);
			cRenderer::renderLine(m_terrain->getVector(vindex[1]), m_terrain->getVector(vindex[2]), 0xff0000ff);
			cRenderer::renderLine(m_terrain->getVector(vindex[2]), m_terrain->getVector(vindex[0]), 0xff0000ff);
		}
		cTerrain const* m_terrain;
	};

	/*
	 *	
	 */
	cPick::cPick() : m_type(CLOSER)
	{
	}
	/*
	 *	
	 */
	cPick::~cPick()
	{
		m_terrainResult.clear();
	}
	/*
	 *	
	 */
	void cPick::render(cTerrain const* /*terrain*/)
	{
		_getRenderPropertyMgr()->begin();
		_getRenderPropertyMgr()->setProperty(sRenderProperty::ZENABLE, FALSE);

		// terrain result
		//for_each(m_terrainResult.m_octreeList.begin(), m_terrainResult.m_octreeList.end(), sRenderOctreeNode());
		//for_each(m_terrainResult.m_rdList.begin(), m_terrainResult.m_rdList.end(), sRenderRdList(terrain));

		if (m_terrainResult.m_picked)
		{
			cRenderer::renderLine(m_terrainResult.m_pickVertex[0], m_terrainResult.m_pickVertex[1], 0xffff0000);
			cRenderer::renderLine(m_terrainResult.m_pickVertex[1], m_terrainResult.m_pickVertex[2], 0xffff0000);
			cRenderer::renderLine(m_terrainResult.m_pickVertex[2], m_terrainResult.m_pickVertex[0], 0xffff0000);
		}

		_getRenderPropertyMgr()->end();
	}
	/*
	 *	
	 */
	int cPick::renderDebugInfo(int y)
	{
		y += 16;
		static TCHAR str[MAX_PATH];
		_getFontMgr()->drawD3DText(10, y+=16, _T("[pick]"), cD3DFont::HEIGHT_14, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		_stprintf(str, _T("pick len: %f"), m_terrainResult.m_t);
		_getFontMgr()->drawD3DText(10, y+=16, str);
		_stprintf(str, _T("cell index : %d"), m_terrainResult.m_cellIndex);
		_getFontMgr()->drawD3DText(10, y+=16, str);
		_stprintf(str, _T("t:%.3f, u:%.3f, v:%.3f"), m_terrainResult.m_t, m_terrainResult.m_u, m_terrainResult.m_v);
		_getFontMgr()->drawD3DText(10, y+=16, str);

		return y;
	}
	/*
	 *	
	 */
	bool cPick::begin(cCamera* camera, int mx, int my)
	{
		//if (!GetCapture())
		//	return false;

		sMatrix4 const& matProj = camera->getProj();
		float width = (float)_getEngineConfig()->getWidth();
		float height = (float)_getEngineConfig()->getHeight();

		// Compute the vector of the pick ray in screen space
		sVector3 v;
		v.x = (((2.0f * (float)mx) / width) - 1.0f) / matProj.m[0][0];
		v.y = -(((2.0f * (float)my) / height) - 1.0f) / matProj.m[1][1];
		v.z = 1.0f;

		// Get the inverse view matrix
		sMatrix4 const& matView = camera->getView();
		sMatrix4 const& matWorld = camera->getWorld();
		sMatrix4 matWV;
		mulMat4(matWorld, matView, matWV);
		sMatrix4 m;
		matWV.inverse(m);
						
		// Transform the screen space pick ray into 3D space
		m_ray.m_dir.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
		m_ray.m_dir.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
		m_ray.m_dir.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];
		m_ray.m_orig.x = m.m[3][0];
		m_ray.m_orig.y = m.m[3][1];
		m_ray.m_orig.z = m.m[3][2];

		return true;
	}
	/*
	 *	
	 */
	bool cPick::pickTerrainFace(cTerrain const* terrain)
	{
		m_terrainResult.clear();
		m_terrainResult.m_picked = pickTerrainFace(terrain, m_ray, m_terrainResult, false, false);
		return m_terrainResult.m_picked;
	}
	/*
	 *	
	 */
	bool cPick::pickTerrainFace(cTerrain const* terrain, sRay const& ray, sPickResult& pickResult, 
								bool dirIsTop, bool checkIsInFace)
	{
		if (!terrain)
			return false;

		pickOctreeNode(ray, terrain->getOctree().getRoot(), pickResult, dirIsTop);

		float t, u, v;
		char temp[NAME_LEN];
		cit_octreeNode it = pickResult.m_octreeList.begin();
		for (; it != pickResult.m_octreeList.end(); ++it)
		{
			cOctreeNode::cit_rdlist it_r = (*it)->m_rdList.begin();
			for (; it_r != (*it)->m_rdList.end(); ++it_r)
			{
				_getStringHelper()->w2m(it_r->first.c_str(), temp, NAME_LEN);
				uint cellIndex = atoi(temp);
				uint const* vindex = terrain->getVertIndex(cellIndex);
				sVector3 const& v0 = terrain->getVector(vindex[0]);
				sVector3 const& v1 = terrain->getVector(vindex[1]);
				sVector3 const& v2 = terrain->getVector(vindex[2]);

				if (checkIsInFace)
				{
					if (!cMath::isInFace(v0, v1, v2, ray.m_orig))
						continue;
				}

				if (intersectTriangle(ray, v0, v1, v2, t, u, v))
				{
					// 픽킹된 위치가 반직선과 반대로 있는 경우는 패스
					if (0 > t)
						continue;
					pickResult.setFind(cellIndex, t, u, v, v0, v1, v2);
					return true;
				}
			}
		}

		return false;
	}
	/*
	 *	
	 */
	bool cPick::pickTerrainHeight(cTerrain const* terrain, sVector3 const& orig, float& height, uint& cellIndex)
	{
		sVector3 dir(0.0f, -1.0f, 0.0f);
		sRay ray(orig, dir);
		sPickResult pickResult;
		if (!pickTerrainFace(terrain, ray, pickResult, false, true))
			return false;

		height = orig.y - pickResult.m_t;
		cellIndex = pickResult.m_cellIndex;

		return true;
	}
	/*
	 *	
	 */
	bool cPick::pickOctreeNode(sRay const& ray, cOctreeNode const* octreeNode, cPick::sPickResult& pickResult, bool dirIsTop)
	{
		if (!octreeNode)
			return false;

		if (!pickBox(ray, octreeNode->getBound()))
			return false;

		// 하위 노드의
		pickResult.m_octreeList.push_back(octreeNode);

		if (dirIsTop)
		{
			for (int i = 0; i < cOctreeNode::REGION_NUM; ++i)
			{
				if (pickOctreeNode(ray, octreeNode->getNode(i), pickResult, dirIsTop))
					break;
			}
		}
		else
		{
			for (int i = 0; i < cOctreeNode::REGION_NUM; ++i)
			{
				pickOctreeNode(ray, octreeNode->getNode(i), pickResult, dirIsTop);
			}
		}

		return true;
	}
	/*
	 *	
	 */
	bool cPick::pickBox(sRay const& ray, sBox const* box)
	{
		float t, u, v;
		sVector3 const* bv = box->m_v;
		// up
		if (intersectTriangle(ray, bv[0], bv[1], bv[2], t, u, v))	return true;
		if (intersectTriangle(ray, bv[1], bv[3], bv[2], t, u, v))	return true;
		// left
		if (intersectTriangle(ray, bv[5], bv[1], bv[4], t, u, v))	return true;
		if (intersectTriangle(ray, bv[1], bv[5], bv[4], t, u, v))	return true;
		// right
		if (intersectTriangle(ray, bv[2], bv[6], bv[3], t, u, v))	return true;
		if (intersectTriangle(ray, bv[6], bv[2], bv[3], t, u, v))	return true;
		// front
		if (intersectTriangle(ray, bv[4], bv[0], bv[2], t, u, v))	return true;
		if (intersectTriangle(ray, bv[0], bv[4], bv[2], t, u, v))	return true;
		// back
		if (intersectTriangle(ray, bv[7], bv[3], bv[5], t, u, v))	return true;
		if (intersectTriangle(ray, bv[3], bv[7], bv[5], t, u, v))	return true;
		// bottom
		if (intersectTriangle(ray, bv[4], bv[5], bv[6], t, u, v))	return true;
		if (intersectTriangle(ray, bv[5], bv[7], bv[6], t, u, v))	return true;

		return false;
	}
	/*
		반직선과 삼각형과의 교차점 계산
		t : 반직선 시작 점과 삼각형과의 충돌 거리
		u, v : 교차점을 표현할 때 쓰이는 무게중심 값들
	*/
	bool cPick::intersectTriangle(sRay const& ray, sVector3 const& v0, sVector3 const& v1, sVector3 const& v2,
								  float& t, float& u, float& v)
	{
		// t : 반직선 시작 점과 삼각형과의 충돌 거리
		// u : 
		// v : 
		// Find vectors for two edges sharing vert0
		sVector3 edge1, edge2;
		subVec3(v1, v0, edge1);
		subVec3(v2, v0, edge2);

		// Begin calculating determinant - also used to calculate U parameter
		sVector3 pvec;
		ray.m_dir.cross(edge2, pvec);

		// If determinant is near zero, ray lies in plane of triangle
		float det = edge1.dot(pvec);

		// det < 0.0f면 백 페이스이다
		sVector3 tvec;
		if( det > 0 )
		{
			subVec3(ray.m_orig, v0, tvec);
		}
		else
		{
			subVec3(v0, ray.m_orig, tvec);
			det = -det;
		}

		if( det < 0.0001f )
			return false;

		// Calculate U parameter and test bounds
		u = tvec.dot(pvec);
		if( u < 0.0f || u > det )
			return false;

		// Prepare to test V parameter
		sVector3 qvec;
		tvec.cross(edge1, qvec);

		// Calculate V parameter and test bounds
		v = ray.m_dir.dot(qvec);
		if( v < 0.0f || u + v > det )
			return false;

		// Calculate t, scale parameters, ray intersects triangle
		t = edge2.dot(qvec);
		FLOAT fInvDet = 1.0f / det;
		t *= fInvDet;
		u *= fInvDet;
		v *= fInvDet;

		return true;
	}
}