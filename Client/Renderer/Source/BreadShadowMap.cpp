
#include "BreadShadowMap.h"
#include "BreadRenderToTexture.h"
#include "BreadUberShaderManager.h"
#include "BreadLightManager.h"
#include "BreadCameraManager.h"
#include "BreadGui.h"
#include "BreadEntityManager.h"
#include "BreadSceneManager.h"
#include "BreadMaterial.h"
#include "BreadRenderPropertyManager.h"
#include "BreadCore.h"
#include "BreadShaderList.h"
#include "BreadDebugRenderManager.h"
#include "BreadRenderData.h"

namespace Bread
{
	static float ZNEAR_MIN = 1.0f;
	static float ZFAR_MAX = 800.0f;
	/*
	*/
	cShadowBase* createShadowMap()
	{
		return new cShadowMap;
	}
	/*
	*	
	*/
	cShadowMap::cShadowMap() : m_rttDepth(NULL), m_shader(NULL), m_isPsm(true)
	{
	}
	/*
	*	
	*/
	cShadowMap::~cShadowMap()
	{
		safeDelete(m_rttDepth);
		safeDelete(m_shader);
	}
	/*
	*	
	*/
	bool cShadowMap::initialize(uint width, uint height)
	{
		cShadowBase::initialize(width, height);

		initRtt();
		initMatrix();
		if (!initShader())
			return false;

		m_zNear = ZNEAR_MIN;
		m_zFar = ZFAR_MAX;
		m_sbNear = 5.0f;
		m_sbFar = 50.0f;

		return true;
	}
	/*
	*	
	*/
	void cShadowMap::initRtt()
	{
		m_rttDepth = new cRenderToTextureNAA;
		m_rttDepth->initialize(0, m_width, m_height, true, D3DFMT_R32F);
	}
	/*
	*	
	*/
	void cShadowMap::initMatrix()
	{
		float offsetX = 0.5f + (0.5f/(float)m_width);
		float offsetY = 0.5f + (0.5f/(float)m_height);
		m_matScaleBias = sMatrix4(0.5f, 0.0f, 0.0f, 0.0f,
									   0.0f,-0.5f, 0.0f, 0.0f,
									   0.0f, 0.0f, 1.0f, 0.0f,
									   offsetX, offsetY, 0.0f, 1.0f);
	}
	/*
	*	
	*/
	bool cShadowMap::initShader()
	{
		assert(!m_shader);
		m_shader = new cShadowMapDepthShader;
		if (!m_shader->createEffect(_T("shadowmapdepth.fx"), cD3DSystem::getD3DDevice(), cShader::makeFlags()))
			return false;
		m_shader->setUserData(this);

		m_depthBias = 100.0f;
		m_shader->setFloat(m_shader->m_hDepthBias, m_depthBias);
		return true;
	}
	/*
	*	
	*/
	void cShadowMap::update(float /*elapsedtime*/)
	{
		cCamera* cam = _getCameraMgr()->getMain();

		sVector3 offsetV(0.0f, 0.0f, 0.0f);
		if (_getInputSystem()->isPressedKey(DIK_T))
			cam->calculMoveOffset(cCamera::MOVE_FORWARD, 2, offsetV);
		if (_getInputSystem()->isPressedKey(DIK_G))
			cam->calculMoveOffset(cCamera::MOVE_BACKWARD, 2, offsetV);
		if (_getInputSystem()->isPressedKey(DIK_F))
			cam->calculMoveOffset(cCamera::MOVE_LEFTSTEP, 2, offsetV);
		if (_getInputSystem()->isPressedKey(DIK_H))
			cam->calculMoveOffset(cCamera::MOVE_RIGHTSTEP, 2, offsetV);
		if (_getInputSystem()->isPressedKey(DIK_R))
			offsetV.y = 0.5f;
		if (_getInputSystem()->isPressedKey(DIK_Y))
			offsetV.y = -0.5f;

		float offset = 0.1f;
		// z near
		if (_getInputSystem()->isPressedKey(DIK_O))
			m_sbNear += offset;
		if (_getInputSystem()->isPressedKey(DIK_P))
			m_sbNear -= offset;

		offset = 1.0f;
		// z far
		if (_getInputSystem()->isPressedKey(DIK_K))
			m_sbFar += offset;
		if (_getInputSystem()->isPressedKey(DIK_L))
			m_sbFar -= offset;

		// depth bias
		if (_getInputSystem()->isPressedKey(DIK_N))
			m_depthBias += 1.0f;
		if (_getInputSystem()->isPressedKey(DIK_M))
			m_depthBias -= 1.0f;

		cEntityModel* entity = _getEntityMgr()->get(_T("camtarget"));
		if (entity)
		{
			cLight* light = _getLightMgr()->get(0);
			sVector3 pos = light->getPosition();
			light->setPosition(sVector3(pos.x+offsetV.x, pos.y+offsetV.y, pos.z+offsetV.z));
			entity->setPosition(light->getPosition());
		}

		m_shader->setFloat(m_shader->m_hDepthBias, m_depthBias);

		if (m_isPsm)	buildPsm();
		else			buildOriginal();
	}
	/*
	*	
	*/
	void cShadowMap::buildOriginal()
	{
		cLight* light = _getLightMgr()->get(0);
		if (light)
		{
			/*
			gpg 책을 보고 적용해 봤는데, 잘 안됨,,왜 안되지,,

			sMatrix4 matLightView;
			cCamera* cam = _getCameraMgr()->getMain();
			sVector3 lightDir(2.0f, 2.0f, 2.0f);
			//light->getDirection().toD3D(lightDir);
			sVector4 _lightDir(lightDir.x, lightDir.y, lightDir.z, 0.0f);
			D3DXVec4Transform(&_lightDir, &_lightDir, &cam->getVP());
			sVector3 lightPos;
			// 광원의 위치 타입
			// 1 평행 광원
			if (cMath::isZero(_lightDir.w))
			{
				lightPos = sVector3(-_lightDir.x, -_lightDir.y, 0.5f);

			}
			// 2 
			else if (_lightDir.w > 0.0f)
			{
				float invW = cMath::inv(_lightDir.w);
				lightPos = sVector3(_lightDir.x*invW, _lightDir.y*invW, _lightDir.z*invW);
			}
			// 3 표준 그림자 맵
			else if (_lightDir.w < 0.0f)
			{
				sVector3 eye;
				light->getPosition().toD3D(eye);
				sVector3 at = sVector3(0.0f, 0.0f, 0.0f);
				sVector3 up = sVector3(0.0f, 1.0f, 0.0f);

				D3DXMatrixLookAtLH(&matLightView, &eye, &at, &up);
				D3DXMatrixPerspectiveFovLH(&m_matLightProj, 1.0f, 1.0f, m_zNear, m_zFar);

				m_matLightVP = matLightView * m_matLightProj;
				return ;
			}

			sVector3 center(0.0f, 0.0f, 0.5f);
			sVector3 up(0.0f, 1.0f, 0.0f);
			D3DXMatrixLookAtLH(&matLightView, &lightPos, &center, &up);

			sVector3 vLen = center - lightPos;
			float dist = D3DXVec3Length(&vLen);
			float radius = 1.5f;
			float angle = 2.0f * asinf(radius / dist);
			float zNear = dist - radius * 2.f;
			float zFar = dist + radius * 2.f;
			if (zNear < 0.001f)	zNear = 0.001f;

			//zNear = 0.001f;//m_sbNear;
			//zFar = 1.0f;//m_sbFar;
			angle = 2.0f;

			D3DXMatrixPerspectiveFovLH(&m_matLightProj, angle, 1.0f, zNear, zFar);
			sMatrix4 vp = matLightView * m_matLightProj;
			m_matLightVP = cam->getVP() * vp;
*/



			sMatrix4 matLightView;
			sVector3 eye, at, up, dir;

			eye = light->getPosition();
			dir = light->getDirection();
			at = sVector3(0.0f, 0.0f, 0.0f);
			up = sVector3(0.0f, 1.0f, 0.0f);

			matLightView.makeViewMatrix(eye, at);
			m_matLightProj.makePerspectiveMatrix(1.0f, 1.0f, m_zNear, m_zFar);
			mulMat4(matLightView, m_matLightProj, m_matLightVP);
		}
	}
	/*
	*	
	*/
	void cShadowMap::buildPsm()
	{
		cCamera* cam = _getCameraMgr()->getMain();
		cLight* light = _getLightMgr()->get(0);

		sVector3 lightDir;
		lightDir = light->getDirection();

		sMatrix4 matViewPrime, matProjPrime, matViewProjPrime;

		// Compute camera view & proj matrices as used by the shadowbuffer
		// mapping.  Does not necessarily have to match true view & proj.

		// Use normal camera view.
		matViewPrime = cam->getView();

		float viewSlideBack = 0.f;

		// More aggressive clipping planes for the shadow buffer.
		computeVirtualCameraParameters();

		// Measure how similar the light & camera directions are.
		float 	dot = lightDir.x * matViewPrime.m[0][2]
					+ lightDir.y * matViewPrime.m[1][2]
					+ lightDir.z * matViewPrime.m[2][2];


		// Some not-very-principled hackery (but the results are
		// very nice): tweak the view to make the projection more
		// orthographic, when the light is aligned with the view
		// vector.  In the aligned situation, we don't want so
		// much perspective effect.

		// -50.0f <= viewSlideBack <= 50.0f
		viewSlideBack += 50.0f * fabsf(dot);


		// 반대방향 일때
		if (dot < 0) 
		{
			// light pointed at camera: we don't need sb info from behind the camera...
			viewSlideBack += 5.f; // -45.0f <= viewSlideBack <= 5.0f가 됨
			m_sbNear += viewSlideBack;
		}
		else
		{
			// light pointed along the camera (light is behind the
			// camera).  Need info from behind the camera, but the SB
			// doesn't need much perspective correction...
			viewSlideBack += 5.f; // 5.0f <= viewSlideBack <= 55.0f가 됨
			m_sbNear += viewSlideBack;
			viewSlideBack = viewSlideBack * 2.f - 5.f;	// xxxxxx, 5.0f <= viewSlideBack <= 105.0f가 됨
		}

		matViewPrime.m[3][2] += viewSlideBack;

		float	angle = atanf(m_sbFar * tanf(D3DX_PI/4) / (m_sbFar + viewSlideBack));

		m_sbFar += viewSlideBack;

		// Make a new projection, with more aggressive c_near/c_far.
		matProjPrime.makePerspectiveMatrix(angle, cam->getAspect(), m_sbNear, m_sbFar);
		// Combine the matrices.
		mulMat4(matViewPrime, matProjPrime, matViewProjPrime);

		// Generate the shadow buffer transform matrix and the
		// corresponding texture transform matrix (the shadow buffer
		// matrix maps from screen space to shadow-buffer pre-projection
		// space; the texture transform matrix just tacks on the extra
		// texture scaling.)
		//
		// Note: if you are using multiple lights & shadow buffers, this needs
		// to be calculated for each light.

		// Light position in post-perspective screen space.
		sVector3	lightPos;

		sMatrix4	view, proj;	// shadowbuffer's viewing matrices, from *screen* space.
		sVector4 _lightDir(lightDir.x, lightDir.y, lightDir.z, 0);

		// Push lightPos through the transformation
		// pipeline to generate a screen-space position
		// for the light.
		lightPos = transformWorldToScreen(_lightDir, matViewProjPrime);
		if (cMath::isZero(lightPos.z))
		{
			// Light behind near clip -- the correct thing
			// to do here is project it to its inverse
			// position on the infinity plane, and then
			// reverse the sense of the shadowbuffer depth
			// test.

			// For a directional light, basically we want to look at the
			// screen-space cube from some point on the "infinite plane"
			// (past the end of zFar).
			float	infinityZ = m_zFar/(m_zFar - m_zNear);

			// Light is pointed (almost) exactly perpendicular to
			// the camera direction.  Generate a light pos that
			// points roughly the right direction, and is very far
			// from the screen box.
			sVector4	pos;
			matViewProjPrime.transform(_lightDir, pos);
			pos.z = 0;
			pos.w = 0;
			pos.normalize();
			lightPos.x = pos.x * 1000.f;
			lightPos.y = pos.y * 1000.f;
			lightPos.z = infinityZ;
		}

		sVector3	center(0.f, 0.f, 0.5f);
		sVector3	zaxis(0.f, 0.f, 1.f);
		float	fRadius = 1.0f;
		sVector3 temp;
		subVec3(lightPos, center, temp);
		float	fDist = temp.length();

		// Look at the screen-box, from the screen-space light position.

		view.makeViewMatrix(lightPos, center);

		float	fAngle = 2.0f * asinf(fRadius / fDist);
		float	n = fDist - fRadius * 2.f;
		float	f = fDist + fRadius * 2.f;
		if (n < 0.001f) { n = 0.001f; }
		proj.makePerspectiveMatrix(fAngle, 1.0f, n, f);

		// Compose matrices into m_matShadowBuffer.
		sMatrix4 matVP;
		mulMat4(view, proj, matVP);
		mulMat4(matViewProjPrime, matVP, m_matLightVP);
	}
	/*
	*	
	*/
	sVector3 cShadowMap::transformWorldToScreen(const sVector4& vec, sMatrix4 const& matViewProjPrime)
	{
		sVector4	result(0, 0, 0, 1);
		matViewProjPrime.transform(vec, result);
		if (result.w < 0.001f) {
			// Hm.
			return sVector3(0, 0, 0);
		}

		float	recip = 1.0f / result.w;
		result.x *= recip;
		result.y *= recip;
		result.z *= recip;
		result.w = 1;

		return sVector3(result.x, result.y, result.z);
	}
	/*
	*/
	struct sCalcMinMax
	{
		sCalcMinMax(sMatrix4 const& modelView) : m_modelView(modelView) {}
		void operator()(cEntityModel* entity)
		{
			cRenderData* rd = _getSceneMgr()->getRenderData(entity->getRenderDataBuid());
			//rd->m_cullFigure.m_sphere.m_center.to3(m_temp);
			//D3DXVec3TransformCoord(&m_temp, &m_temp, &m_modelView);
			m_temp = rd->m_cullFigure.m_sphere.m_center;
			sVector3 out;
			m_modelView.transformCoord(m_temp, out);

			min_z = min(min_z, out.z - 10.0f);
			max_z = max(max_z, out.z + 10.0f);
		}
		static float min_z;
		static float max_z;
		sMatrix4 m_modelView;
		sVector3 m_temp;
	};
	float sCalcMinMax::min_z = 1e32f;
	float sCalcMinMax::max_z = 0.0f;
	/*
	*	
	*/
	void cShadowMap::computeVirtualCameraParameters()
	{
		cCamera* cam = _getCameraMgr()->getMain();

		sMatrix4 modelView;
		mulMat4(cam->getWorld(), cam->getView(), modelView);

		float min_z, max_z;
				
		//_getEntityMgr()->forEach(sCalcMinMax(modelView));
		_getEntityMgr()->forEach(sCalcMinMax(modelView));
		min_z = sCalcMinMax::min_z;
		max_z = sCalcMinMax::max_z;

		m_sbNear = max(ZNEAR_MIN, min_z);
		m_sbFar = min(ZFAR_MAX, max_z);
	}
	/*
	*	
	*/
	void cShadowMap::beforeRender3D()
	{
		renderDepth();
	}
	/*
	*	
	*/
	void cShadowMap::afterRender2D()
	{
		_getDebugRenderMgr()->addScreen(800.0f, 0.0f, 128.0f, 128.0f, m_rttDepth->getTexture());
	}
	/*
	*	
	*/
	void cShadowMap::renderDepth()
	{
		//float depthBias = 0.001f;//20.0f/16777215.0f;
		//float slopeScaleDepthBias = 0.0f;

		_getRenderPropertyMgr()->begin();
		//_getRenderPropertyMgr()->setProperty(sRenderProperty::DEPTHBIAS, *(DWORD*)&depthBias);
		//_getRenderPropertyMgr()->setProperty(sRenderProperty::SLOPESCALEDEPTHBIAS, *(DWORD*)&slopeScaleDepthBias);

		cRenderer::renderSingleScene(m_rttDepth, 1, m_shader, _T("TDepth"), true);

		_getRenderPropertyMgr()->end();
	}
	/*
	*	
	*/
	int cShadowMap::renderDebugInfo(int y)
	{
		TCHAR str[MAX_PATH];

		_stprintf(str, _T("zNear: %f, %f"), m_zNear, m_sbNear);
		_getFontMgr()->drawD3DText(10, y+=16, str);

		_stprintf(str, _T("zFar: %f, %f"), m_zFar, m_sbFar);
		_getFontMgr()->drawD3DText(10, y+=16, str);

		_stprintf(str, _T("depthBias: %f"), m_depthBias);
		_getFontMgr()->drawD3DText(10, y+=16, str);

		return y;
	}


}







