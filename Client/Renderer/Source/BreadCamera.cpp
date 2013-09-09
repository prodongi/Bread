
#include "BreadCamera.h"
#include "BreadUtils.h"
#include "BreadCore.h"
#include "BreadGui.h"
#include "BreadEntityManager.h"
#include "BreadStringHelper.h"
#include "BreadVector2.h"
#include "BreadFrustumPlane.h"
#include "BreadFrustumRadar.h"

namespace Bread
{
	float const cCamera::DEFAULT_WHEEL_SENSITIVE = 0.01f;
	/*
	 *	
	 */
	cCamera::cCamera() : m_type(TYPE_BACK), m_frustum(NULL)
	{
		init();
	}
	/*
	 *	
	 */
	cCamera::cCamera(TCHAR* name) : m_name(name), m_type(TYPE_BACK), m_frustum(NULL)
	{
		init();
	}
	/*
	 *	
	 */
	cCamera::~cCamera()
	{
		safeDelete(m_frustum);
	}
	/*
	 *	
	 */
	void cCamera::init()
	{
		initFrustum();

		// 가장 먼저 초기화 되야 된다.
		m_wheelSensitive = DEFAULT_WHEEL_SENSITIVE;
		setSpeed(1.0f);
		setProjParam(cMath::PI/4.0f, 
					(float)_getEngineConfig()->getWidth()/(float)_getEngineConfig()->getHeight(),
					_getEngineConfig()->getZNear(), 
					_getEngineConfig()->getZFar());
		setMinAtToEyeLength((float)DEFAULT_MIN_AT_TO_EYE_LEN);
		setMaxAtToEyeLength((float)DEFAULT_MAX_AT_TO_EYE_LEN);
		m_up = sVector3(0.0f, 1.0f, 0.0f);
		// 초기 위치는 임의로 해준다
		set(sVector3(300.0f, 100.0f, 100.0f), sVector3(0.0f, 0.0f, 0.0f), 30.0f);
	}
	/*
	*/
	void cCamera::initFrustum()
	{
		m_frustum = new cFrustumPlane;
		//m_frustum = new cFrustumRadar;
	}
	/*
	 *	
	 */
	void cCamera::set(sVector3 const& eye, sVector3 const& at, float targetHeight)
	{
		m_eye = eye;
		m_at = at;
		setLookAtLH();
		calculAtToEyeLength();
		m_targetHeight = targetHeight;
	}
	/*
	 *	
	 */
	void cCamera::setEye(float atToEyeLength)
	{
		m_atToEyeLength = atToEyeLength;
		sVector3 temp;
		mulVec3(sVector3(m_view.m[0][2], m_view.m[1][2], m_view.m[2][2]), atToEyeLength, temp);
		subVec3(m_at, temp, m_eye);
	}
	/*
	 *	
	 */
	void cCamera::setProjParam(float fov, float aspect, float znear, float zfar)
	{
		m_fov = fov;
		m_aspect = aspect;
		m_zNear = znear;
		m_zFar = zfar;
		updateProjectMatrix();

		if (m_frustum)
			m_frustum->setProjParam(fov, aspect, znear, zfar);
	}
	/*
	 *	
	 */
	void cCamera::updateProjectMatrix()
	{
		m_proj.makePerspectiveMatrix(m_fov, m_aspect, m_zNear, m_zFar);
	}
	/*
	 *	
	 */
	void cCamera::setTransform(LPDIRECT3DDEVICE9 device)
	{
		// world
		m_world.identity();
		D3DXMATRIXA16 matWorld;
		m_world.to(matWorld);
		device->SetTransform(D3DTS_WORLD, &matWorld);

		// view
		D3DXMATRIXA16 matView;
		m_view.to(matView);
		device->SetTransform(D3DTS_VIEW, &matView);

		// projection
		D3DXMATRIXA16 matProj;
		m_proj.to(matProj);
		device->SetTransform(D3DTS_PROJECTION, &matProj);

		m_frustum->construct(m_view, m_proj, m_eye);
	}
	/*
	 *	
	 */
	LRESULT CALLBACK cCamera::wndProc(HWND , UINT , WPARAM , LPARAM )
	{
		return 1;
	}
	/*
	 *	
	 */
	void cCamera::update(float /*elapsedtime*/)
	{
		updateTargetEntityPos();
		updateTranslation();
		updateAtEntity();

		mulMat4(m_view, m_proj, m_vp);
	}
	/*
	 *	
	 */
	void cCamera::updateTargetEntityPos()
	{
		if (TYPE_BACK != m_type)
			return ;
		if (m_targetEntity.empty())
			return ;
		cEntityModel* entity = _getEntityMgr()->get(m_targetEntity);
		if (!entity)
			return ;

		m_at = entity->getPosition();
		m_at.y += m_targetHeight;
		setEye(m_atToEyeLength);
		setLookAtLH();
	}
	/*
	 *
	 */
	void cCamera::updateAtEntity()
	{
		if (m_atEntity.empty())
			return ;
		cEntityModel* entity = _getEntityMgr()->get(m_atEntity);
		entity->setPosition(m_at);
	}
	/*
	 *	
	 */
	void cCamera::updateTranslation()
	{
		if (TYPE_BACK == m_type)
			updateTranslationBack();
		else if (TYPE_FPS == m_type)
			updateTranslationFps();
	}
	/*
	 *	
	 */
	void cCamera::updateTranslationFps()
	{
	}
	/*
	 *	
	 */
	void cCamera::updateTranslationBack()
	{
		if (_getInputSystem()->getMouseMove())
		{
			int dx = _getInputSystem()->getMouseDx();
			int dy = _getInputSystem()->getMouseDy();

			if (_getInputSystem()->isLButtonDown())
				rotation(sVector3((float)dx, (float)dy, 0.0f));
			else if (_getInputSystem()->isMButtonDown())
			{
				if (m_targetEntity.empty())
				{
					if (dx < 0)
						move(MOVE_RIGHTSTEP, abs(dx));
					else if (dx > 0)
						move(MOVE_LEFTSTEP, abs(dx));
					if (dy < 0)
						move(MOVE_UP, abs(dy));
					else if (dy > 0)
						move(MOVE_DOWN, abs(dy));
				}
			}
		}

		if (_getInputSystem()->getMouseWheel())
			zoom(_getInputSystem()->getWheelDelta());

		if (m_targetEntity.empty())
		{
			if (_getInputSystem()->isPressedKey(DIK_S))
				move(MOVE_BACKWARD);
			if (_getInputSystem()->isPressedKey(DIK_W))
				move(MOVE_FORWARD);
			if (_getInputSystem()->isPressedKey(DIK_D))
				move(MOVE_RIGHTSTEP);
			if (_getInputSystem()->isPressedKey(DIK_A))
				move(MOVE_LEFTSTEP);
		}
	}
	/*
	 *	
	 */
	void cCamera::move(unsigned int dir, int offset)
	{
		sVector3 offsetV;
		calculMoveOffset(dir, offset, offsetV);
		m_eye += offsetV;
		m_at  += offsetV;

		setLookAtLH();
	}
	/*
	 *	
	 */
	void cCamera::calculMoveOffset(unsigned int dir, int offset, sVector3& offsetV)
	{
		sVector3 vDir;
		switch (dir)
		{
		case MOVE_FORWARD:
		case MOVE_BACKWARD:		vDir = sVector3(m_view.m[0][2], 0.0f, m_view.m[2][2]);	break;
		case MOVE_LEFTSTEP:
		case MOVE_RIGHTSTEP:	vDir = sVector3(m_view.m[0][0], 0.0f, m_view.m[2][0]);	break;
		case MOVE_UP:
		case MOVE_DOWN:			vDir = sVector3(m_view.m[0][1], m_view.m[1][1], m_view.m[2][1]);	break;
		}

		// 0.005f는 테스트후에 나온 수치이다.
		sVector3 len;
		subVec3(m_eye, m_at, len);
		float l = len.length() * 0.005f;

		float v = m_speed * (float)offset * l;
		switch (dir)
		{
		case MOVE_BACKWARD:	v = -v;	break;
		case MOVE_LEFTSTEP:	v = -v; break;
		case MOVE_UP:		v = -v; break;
		}

		vDir.normalize();

		mulVec3(vDir, v, offsetV);
	}
	/*
	 *	
	 */
	void cCamera::rotation(sVector3 const& delta)
	{
		float   yaw;
		float   pitch;

		sVector3 vZ;
		sVector3 vY;
		sVector3 vX;

		sMatrix4 matY;
		sMatrix4 matX;

		// 월드 좌표 y 축에 대한 회전
		yaw = cMath::degToRad(delta.x * m_speed);
		matY.rotationY(yaw);

		if (TYPE_FPS == getType())
			subVec3(m_at, m_eye, vZ);
		else if (TYPE_BACK == getType())
			subVec3(m_eye, m_at, vZ);
		vY = sVector3(m_view.m[0][1], m_view.m[1][1], m_view.m[2][1]);

		matY.transformCoord(vZ, vZ);
		matY.transformCoord(vY, vY);
		
		if (TYPE_FPS == getType())
			addVec3(vZ, m_eye, m_at);
		else if (TYPE_BACK == getType())
			addVec3(m_at, vZ, m_eye);
		m_up = vY;

		setLookAtLH();

		// 카메라의 x 축에 대한 회전
		pitch  = cMath::degToRad(delta.y * m_speed);
		vX = sVector3(m_view.m[0][0], m_view.m[1][0], m_view.m[2][0]);
		vY = sVector3(m_view.m[0][1], m_view.m[1][1], m_view.m[2][1]);
		
		if (TYPE_FPS == getType())
			subVec3(m_at, m_eye, vZ);
		else if (TYPE_BACK == getType())
			subVec3(m_eye, m_at, vZ);

		// 축 회전 함수가 없어서 이렇게 함
		D3DXMATRIXA16 _matX;
		D3DXVECTOR3  _vX;
		vX.to3(_vX);
		D3DXMatrixRotationAxis(&_matX, &_vX, pitch);
		matX = _matX;
		matX.transformCoord(vZ, vZ);
		matX.transformCoord(vY, vY);

		if (TYPE_FPS == getType())
			addVec3(vZ, m_eye, m_at);
		else if (TYPE_BACK == getType())
			addVec3(m_at, vZ, m_eye);
		m_up  = vY;

		setLookAtLH();
	}
	/*
	 *	
	 */
	void cCamera::zoom(int delta)
	{
		// 일정거리 이하에서는 세밀한 조정을 하기 위해서
		if (m_atToEyeLength < (float)DETAIL_ZOOM_BOUND)
			delta /= 10;
		// 거리에 따른 변화량을 틀리게 해주기 위해서, 100 단위마다 틀리게 해준다
		float mul = m_atToEyeLength * m_wheelSensitive;
		if (mul > 10.0f) mul = 10.0f;
		float L = m_atToEyeLength - (float)delta * mul;
		if (L < m_minAtToEyeLength)
			L = m_minAtToEyeLength;
		else if (L > m_maxAtToEyeLength)
			L = m_maxAtToEyeLength;

		setEye(L);
		setLookAtLH();
	}
	/*
	 *	
	 */
	void cCamera::setLookAtLH()
	{
		m_view.makeViewMatrix(m_eye, m_at);
	}
	/*
	 *	
	 */
	void cCamera::calculAtToEyeLength()
	{
		sVector3 L;
		subVec3(m_eye, m_at, L);
		m_atToEyeLength = L.length();
	}
	/*
	 *	
	 */
	void cCamera::getEyeToAt(sVector3& dir)
	{
		subVec3(m_at, m_eye, dir);
		dir.normalize();
	}
	/*
	*/
	void cCamera::getAxis(sVector3& xAxis, sVector3& yAxis, sVector3& dir)
	{
		getEyeToAt(dir);

		/// xAxis
		yAxis = sVector3(0.0f, 1.0f, 0.0f);
		xAxis.cross(yAxis, dir);
		xAxis.normalize();

		yAxis.cross(dir, xAxis);
		yAxis.normalize();
	}
	/*
	*/
	void cCamera::calculDir(sVector3& dir) const
	{
		subVec3(m_at, m_eye, dir);
		dir.normalize();
	}
	/*
	*/
	void cCamera::getFrustumBox(sBox& box, float zNear, float zFar, float fovYDiv)
	{
		sVector3 camPos, camDir, camA, camB;
		getAxis(camA, camB, camDir);
		camPos = m_eye;

		float fovYDegree = cMath::radToDeg(getFov());
		float fovY = (fovYDegree/2.0f) * fovYDiv;
		float fovX = fovY * getAspect();

		fovY = cMath::degToRad(fovY);
		fovX = cMath::degToRad(fovX);

		float tanTemp;
		tanTemp = cMath::tanf(fovX);	sVector2 camViewWidth(tanTemp*zNear, tanTemp*zFar);
		tanTemp = cMath::tanf(fovY);	sVector2 camViewHeight(tanTemp*zNear, tanTemp*zFar);

		sVector3 leftNear(camA);	leftNear *= camViewWidth.x;
		sVector3 upNear(camB);		upNear *= camViewHeight.x;
		sVector3 leftFar(camA);		leftFar *= camViewWidth.y;
		sVector3 upFar(camB);		upFar *= camViewHeight.y;

		float viewportZShift = 1.0f;

		sVector3 nearCenter(camPos);
		nearCenter += sVector3(camDir * zNear);
		nearCenter += sVector3(camDir * (-viewportZShift));

		sVector3 farCenter(camPos);
		farCenter += sVector3(camDir * zFar);
		farCenter += sVector3(camDir * (-viewportZShift));

		for (int i = 0; i < 4; ++i)
		{
			box.m_v[i] = nearCenter;
			box.m_v[i+4] = farCenter;
		}

		// 1 0    5 4 
		// 3 2    7 6
		box.m_v[0] += leftNear;	box.m_v[0] += upNear;
		box.m_v[1] -= leftNear; box.m_v[1] += upNear;
		box.m_v[2] += leftNear; box.m_v[2] -= upNear;
		box.m_v[3] -= leftNear; box.m_v[3] -= upNear;

		box.m_v[4] += leftFar; box.m_v[4] += upFar;
		box.m_v[5] -= leftFar; box.m_v[5] += upFar;
		box.m_v[6] += leftFar; box.m_v[6] -= upFar;
		box.m_v[7] -= leftFar; box.m_v[7] -= upFar;
	}
	/*
	*/
	int cCamera::renderDebugInfo(int y)
	{
		TCHAR const* str = _getStringHelper()->make(_T("eye:%.2f, %.2f, %.2f at:%.2f, %.2f, %.2f"), m_eye.x, m_eye.y, m_eye.z,
																									m_at.x, m_at.y, m_at.z);
		_getFontMgr()->drawD3DText(10, y+=16, str, cD3DFont::HEIGHT_14, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		return y;
	}
	/*
	*/
	void cCamera::convert3DTo2D(sVector3 const& _3d, sVector2& _2d, bool boundCheck)
	{
		D3DXVECTOR3 _x3d;
		_3d.to3(_x3d);

		D3DVIEWPORT9 viewPort;
		cD3DSystem::getD3DDevice()->GetViewport(&viewPort);

		sVector3 p;
		m_vp.transformCoord(_x3d, p);
		_2d.x = ((1.0f + p.x) * 0.5f) * (float)viewPort.Width + (float)viewPort.X;
		_2d.y = ((1.0f - p.y) * 0.5f) * (float)viewPort.Height + (float)viewPort.Y;

		if (boundCheck)
		{
			if (_2d.x < 0.0f)							_2d.x = 0.0f;
			else if (_2d.x > (float)viewPort.Width)		_2d.x = (float)viewPort.Width;
			if (_2d.y < 0.0f)							_2d.y = 0.0f;
			else if (_2d.y > (float)viewPort.Height)	_2d.y = (float)viewPort.Height;
		}
	}
}

















