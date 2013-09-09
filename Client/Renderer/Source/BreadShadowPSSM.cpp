
#include "BreadShadowPSSM.h"
#include "BreadCameraManager.h"
#include "BreadLightManager.h"
#include "BreadEntityManager.h"
#include "BreadUtils.h"
#include "BreadMath.h"
#include "BreadShaderList.h"
#include "BreadRenderPropertyManager.h"
#include "BreadRenderer.h"
#include "BreadStringHelper.h"
#include "BreadSceneManager.h"
#include "BreadOctree.h"
#include "BreadOctreeNode.h"
#include "BreadGui.h"
#include "BreadDebugRenderManager.h"

namespace Bread
{
	static float _fLightNear=10.0f;
	static float _fLightFar=400.0f; // dynamically adjusted
	static float _fLightFarMax=400.0f;
	static float _fLightFOV=90.0f;
	int cShadowPSSM::splitNum = 5;
	float cShadowPSSM::splitScemeLambda = 0.5f;

	/*
	*/
	cShadowBase* createShadowPSSM()
	{
		return new cShadowPSSM;
	}

	// Builds a standard perspective view and projection matrix
	//
	//
	void CalculateViewProj(sMatrix4 &mView, sMatrix4 &mProj,
						   sVector3 vSource, sVector3 vTarget, sVector3 vUpVector,
		float fFOV, float fNear, float fFar, float fAspect)
	{
		// view matrix
		mView.makeViewMatrix(vSource, vTarget);

		// projection matrix
		mProj.makePerspectiveMatrix(cMath::degToRad(fFOV), fAspect, fNear, fFar);
	}

	// Calculates the view and projection matrix for a light. The projection
	// matrix is "zoomed in" on the given frustum split.
	//
	//
	void CalculateLightForFrustum(sVector3 *pCorners, sMatrix4& matLightProj, sMatrix4& matLightView)
	{
		cLight* light = _getLightMgr()->get(0);
		sVector3 lightSource, lightTarget;
		lightSource = light->getPosition();
		lightTarget = sVector3(0.0f, 0.0f, 0.0f);

		// calculate standard view and projection matrices for light
		CalculateViewProj(matLightView,matLightProj,
			lightSource, lightTarget, sVector3(0,1,0),
			_fLightFOV, _fLightNear, _fLightFarMax,1);


		// Next we will find the min and max values of the current
		// frustum split in lights post-projection space
		// (where coordinate range is from -1.0 to 1.0)
		//
		float fMaxX=-FLT_MAX;
		float fMaxY=-FLT_MAX;
		float fMinX=FLT_MAX;
		float fMinY=FLT_MAX;
		float fMaxZ=0;

		sMatrix4 mLightViewProj;
		mulMat4(matLightView, matLightProj, mLightViewProj);

		// for each corner point
		for(int i=0;i<8;i++)
		{
			// transform point
			sVector4 vTransformed;
			mLightViewProj.transform(sVector4(pCorners[i], 1.0f), vTransformed);

			// project x and y
			vTransformed.x/=vTransformed.w;
			vTransformed.y/=vTransformed.w;

			// find min and max values
			if(vTransformed.x>fMaxX) fMaxX=vTransformed.x;
			if(vTransformed.y>fMaxY) fMaxY=vTransformed.y;
			if(vTransformed.y<fMinY) fMinY=vTransformed.y;
			if(vTransformed.x<fMinX) fMinX=vTransformed.x;

			// find largest z distance
			if(vTransformed.z>fMaxZ) fMaxZ=vTransformed.z;
		}


		// set values to valid range (post-projection)
		clamp(fMaxX,-1.0f,1.0f);
		clamp(fMaxY,-1.0f,1.0f);
		clamp(fMinX,-1.0f,1.0f);
		clamp(fMinY,-1.0f,1.0f);

		// Adjust the far plane of the light to be at the farthest
		// point of the frustum split. Some bias may be necessary.
		//
		_fLightFar=fMaxZ+_fLightNear+1.5f;

		// re-calculate lights matrices with the new far plane
		CalculateViewProj(matLightView,matLightProj,
			lightSource, lightTarget, sVector3(0,1,0),
			_fLightFOV, _fLightNear, _fLightFar, 1);


		// Next we build a special matrix for cropping the lights view
		// to only contain points of the current frustum split
		//

		float fScaleX=2.0f/(fMaxX-fMinX);
		float fScaleY=2.0f/(fMaxY-fMinY);

		float fOffsetX=-0.5f*(fMaxX+fMinX)*fScaleX;
		float fOffsetY=-0.5f*(fMaxY+fMinY)*fScaleY;

		sMatrix4 mCropView(  fScaleX,     0.0f,  0.0f,   0.0f,
								0.0f,  fScaleY,  0.0f,   0.0f,
								0.0f,     0.0f,  1.0f,   0.0f,
								fOffsetX, fOffsetY,  0.0f,   1.0f );

		// multiply the projection matrix with it
		sMatrix4 temp(matLightProj);
		mulMat4(temp, mCropView, matLightProj);

		// finally modify projection matrix for linearized depth
		matLightProj.m[2][2]/=_fLightFar;
		matLightProj.m[3][2]/=_fLightFar;
	}

	// Computes corner points of a frustum
	//
	//
	void CalculateFrustumCorners(sVector3 *pPoints,
		const sVector3 &vSource,
		const sVector3 &vTarget,
		const sVector3 &vUp,
		float fNear, float fFar,
		float fFOV, float fAspect, float fScale)
	{
		sVector3 vZ;
		subVec3(vTarget, vSource, vZ);
		vZ.normalize();

		sVector3 vX;
		vUp.cross(vZ, vX);
		vX.normalize();

		sVector3 vY;
		vZ.cross(vX, vY);

		float fNearPlaneHeight = tanf(fFOV * 0.5f) * fNear;
		float fNearPlaneWidth = fNearPlaneHeight * fAspect;

		float fFarPlaneHeight = tanf(fFOV * 0.5f) * fFar;
		float fFarPlaneWidth = fFarPlaneHeight * fAspect;

		sVector3 vNearPlaneCenter = vSource + vZ * fNear;
		sVector3 vFarPlaneCenter = vSource + vZ * fFar;

		pPoints[0]=sVector3(vNearPlaneCenter - vX*fNearPlaneWidth - vY*fNearPlaneHeight);
		pPoints[1]=sVector3(vNearPlaneCenter - vX*fNearPlaneWidth + vY*fNearPlaneHeight);
		pPoints[2]=sVector3(vNearPlaneCenter + vX*fNearPlaneWidth + vY*fNearPlaneHeight);
		pPoints[3]=sVector3(vNearPlaneCenter + vX*fNearPlaneWidth - vY*fNearPlaneHeight);

		pPoints[4]=sVector3(vFarPlaneCenter - vX*fFarPlaneWidth - vY*fFarPlaneHeight);
		pPoints[5]=sVector3(vFarPlaneCenter - vX*fFarPlaneWidth + vY*fFarPlaneHeight);
		pPoints[6]=sVector3(vFarPlaneCenter + vX*fFarPlaneWidth + vY*fFarPlaneHeight);
		pPoints[7]=sVector3(vFarPlaneCenter + vX*fFarPlaneWidth - vY*fFarPlaneHeight);

		// calculate center of points
		sVector3 vCenter(0,0,0);
		for(int i=0;i<8;i++) vCenter+=pPoints[i];
		vCenter/=8;

		// for each point
		for(int i=0;i<8;i++)
		{
			// scale by adding offset from center
			pPoints[i]+=(pPoints[i]-vCenter)*(fScale-1);
		}
	}

	/*
	*/
	cShadowPSSM::cShadowPSSM() : m_splitInfo(NULL),
								 m_shader(NULL)
	{
	}
	/*
	*/
	cShadowPSSM::~cShadowPSSM()
	{
		safeDeleteArray(m_splitInfo);
		safeDelete(m_shader);
	}
	/*
	*/
	bool cShadowPSSM::initialize(uint width, uint height)
	{
		if (!cShadowBase::initialize(width, height))
			return false;
		//
		m_splitInfo = new sSplitInfo[cShadowPSSM::splitNum + 1];

		if (!initRtt())
			return false;
		if (!initShader())
			return false;
		initEtc();

		return true;
	}
	/*
	*/
	bool cShadowPSSM::initRtt()
	{
		for (int i = 0; i < cShadowPSSM::splitNum; ++i)
		{
			sSplitInfo* sinfo = m_splitInfo + i;
			// rtt
			sinfo->m_rtt = new cRenderToTextureNAA;
			if (!sinfo->m_rtt->initialize(0, m_width, m_height, true, D3DFMT_R32F))
				return false;
		}
		return true;
	}
	/*
	*/
	void cShadowPSSM::initEtc()
	{
		//m_sceneAABB = *_getSceneOctree()->getRoot()->getBound();
	}
	/*
	*/
	bool cShadowPSSM::initShader()
	{
		assert(!m_shader);
		m_shader = new cShadowPSSMShader;
		if (!m_shader->createEffect(_T("shadowPSSM.fx"), cD3DSystem::getD3DDevice(), cShader::makeFlags()))
			return false;
		m_shader->setUserData(this);

		return true;
	}
	/*
	*/
	void cShadowPSSM::update(float /*elapsedtime*/)
	{
		updateNearFar();
		updateSplitDistance();
	}
	/*
	*/
	void cShadowPSSM::updateNearFar()
	{
		if (!_getMainSceneOctree())
			return ;
		cOctreeNode const* rootNode = _getMainSceneOctree()->getRoot();
		if (!rootNode)
			return ;
		cCamera* cam = _getCameraMgr()->getMain();
		float maxZ = 0.0f;
		for (int i = 0; i < sBox::VERT_NUM; ++i)
		{
			sMatrix4 matView = cam->getView();
			sVector3 const* v = rootNode->getBound()->m_v + i;
			float z = v->x * matView.m[0][2] +
				      v->y * matView.m[1][2] +
					  v->z * matView.m[2][2] +
					  1.0f * matView.m[3][2];

			if (z > maxZ) maxZ = z;
		}
		m_camNear = cam->getZNear();
		m_camFar = maxZ + m_camNear;
	}
	/*
	*/
	void cShadowPSSM::updateSplitDistance()
	{
		clamp(cShadowPSSM::splitScemeLambda, 0.0f, 1.0f);

		for (int i = 0; i < cShadowPSSM::splitNum; ++i)
		{
			float idm = (float)i/(float)cShadowPSSM::splitNum;
			float log = m_camNear * powf(m_camFar/m_camNear, idm);
			float uniform = m_camNear + (m_camFar - m_camNear) * idm;
			m_splitInfo[i].m_distance = log * cShadowPSSM::splitScemeLambda + uniform * (1.0f - cShadowPSSM::splitScemeLambda);
		}
		m_splitInfo[0].m_distance = m_camNear;
		m_splitInfo[cShadowPSSM::splitNum].m_distance = m_camFar;
	}
	/*
	*/
	void cShadowPSSM::beforeRender3D()
	{
		cCamera* cam = _getCameraMgr()->getMain();
		sVector3 corners[sBox::VERT_NUM];
		sMatrix4 matLightView, matLightProj;
		sBox box;

		sVector3 eye, at, up;
		eye = cam->getEye();
		at = cam->getAt();
		up = cam->getUp();

		for (int i = 0; i < cShadowPSSM::splitNum; ++i)
		{
			float _near = m_splitInfo[i].m_distance;
			float _far = m_splitInfo[i+1].m_distance;
			float scale = 1.0f;
			CalculateFrustumCorners(corners, eye, at, up, _near, _far, cam->getFov(), cam->getAspect(), scale);

			CalculateLightForFrustum(corners, matLightProj, matLightView);
			mulMat4(matLightView, matLightProj, m_matLightVP);

			cRenderer::renderSingleScene(m_splitInfo[i].m_rtt, 1, m_shader, _T("TShadowPSSM"), true);

			box.m_v[0] = corners[0];
			box.m_v[1] = corners[4];
			box.m_v[2] = corners[7];
			box.m_v[3] = corners[3];
			box.m_v[4] = corners[1];
			box.m_v[5] = corners[5];
			box.m_v[6] = corners[6];
			box.m_v[7] = corners[2];

			cRenderer::renderBox(&box, D3DCOLOR_ARGB(255, 0, 255, 0));
		}
	}
	/*
	*/
	void cShadowPSSM::afterRender2D()
	{
		for (int i = 0; i < cShadowPSSM::splitNum; ++i)
		{
			_getDebugRenderMgr()->addScreen(800.0f, 20.0f+120.0f*(float)i, 128.0f, 128.0f, m_splitInfo[i].m_rtt->getTexture());
		}
	}
	/*
	*/
	int cShadowPSSM::renderDebugInfo(int y)
	{
		TCHAR const* str;
		str = _getStringHelper()->make(_T("near: %.2f, far: %.2f"), m_camNear, m_camFar);
		_getFontMgr()->drawD3DText(10, y+=16, str, cD3DFont::HEIGHT_14, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		return y;
	}
}