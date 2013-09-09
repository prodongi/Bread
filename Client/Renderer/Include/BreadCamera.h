
#ifndef _BreadCamera_h_
#define _BreadCamera_h_

#include <Windows.h>
#include "BreadD3DHeader.h"
#include "BreadDeclaration.h"
#include "BreadEntity.h"

namespace Bread
{
	struct sVector2;
	class cFrustum;

	/*
	*/
	class cCamera : public cEntity
	{
	public:
		enum	{	TYPE_FPS,	TYPE_BACK	};
		enum	{	MOVE_FORWARD,	MOVE_BACKWARD,	MOVE_LEFTSTEP, MOVE_RIGHTSTEP, MOVE_UP, MOVE_DOWN	};
		enum	{	DETAIL_ZOOM_BOUND = 2000	};
		enum	{	DEFAULT_MIN_AT_TO_EYE_LEN = 10, DEFAULT_MAX_AT_TO_EYE_LEN = DETAIL_ZOOM_BOUND	};
		static float const DEFAULT_WHEEL_SENSITIVE;

	public:
		cCamera();
		cCamera(TCHAR* name);
		virtual ~cCamera();
		virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
		virtual void update(float elapsedtime);
		virtual void move(unsigned int dir, int offset = 1);
		virtual void rotation(sVector3 const& delta);
		/// @param delta : 0 < zoom out, 0 > zoom in
		virtual void zoom(int delta);
		void set(sVector3 const& eye, sVector3 const& at, float targetHeight = 0.0f);
		/// at에서 atToEyeLength만큼 떨어진 eye위치를 구한다
		void setEye(float atToEyeLength);
		void setTransform(LPDIRECT3DDEVICE9 device);
		void setProjParam(float fov, float aspect, float znear, float zfar);
		void getEyeToAt(sVector3& dir);
		void getAxis(sVector3& xAxis, sVector3& yAxis, sVector3& dir);
		void calculMoveOffset(unsigned int dir, int offset, sVector3& offsetV);
		void calculDir(sVector3& dir) const;
		int renderDebugInfo(int y);
		void getFrustumBox(sBox& box, float zNear, float zFar, float fovYDiv = 1.0f);
		void convert3DTo2D(sVector3 const& _3d, sVector2& _2d, bool bondCheck = false);

		sMatrix4 const& getWorld()	{	return m_world;	}
		sMatrix4 const& getView()	{	return m_view;	}
		sMatrix4 const& getProj()	{	return m_proj;	}
		sMatrix4 const& getVP()		{	return m_vp;	}
		sVector3 const& getEye()	{	return m_eye;	}
		sVector3 const& getAt()		{	return m_at;	}
		sVector3 const& getUp()		{	return m_up;	}
		TCHAR* getName() const					{	return (TCHAR*)m_name.c_str();	}
		cFrustum const* getFrustum() const		{	return m_frustum;	}
		float getFov() const					{	return m_fov;		}
		float getAspect() const					{	return m_aspect;	}

		propertyConstFunc(uint, Type, m_type)
		propertyConstFunc(float, Speed, m_speed)
		propertyConstFunc(BUID, TargetEntity, m_targetEntity)
		propertyConstFunc(BUID, AtEntity, m_atEntity)
		propertyConstFunc(float, TargetHeight, m_targetHeight)
		propertyConstFunc(float, AtToEyeLength, m_atToEyeLength)
		propertyConstFunc(float, MinAtToEyeLength, m_minAtToEyeLength)
		propertyConstFunc(float, MaxAtToEyeLength, m_maxAtToEyeLength)
		propertyConstFunc(float, ZNear, m_zNear)
		propertyConstFunc(float, ZFar, m_zFar)

	protected:
		virtual void init();
		virtual void initFrustum();
		virtual void updateTranslation();
		virtual void updateTranslationFps();
		virtual void updateTranslationBack();
		void updateProjectMatrix();
		void setLookAtLH();
		void updateTargetEntityPos();
		void updateAtEntity();
		void calculAtToEyeLength();

	protected:
		unsigned int m_type;
		std_string	m_name;
		float m_speed;
		float m_targetHeight;  /// back 카메라의 타겟 높이
		float m_zNear;
		float m_zFar;
		float m_wheelSensitive;	/// 휠 민감도
		float m_fov;
		float m_aspect;
		/// @brief at과 eye와의 거리
		float m_atToEyeLength;
		float m_minAtToEyeLength;
		float m_maxAtToEyeLength;
		BUID m_targetEntity;
		/// @brief m_at의 위치를 나타내줄 entity
		BUID m_atEntity;
	private:
		sMatrix4 m_world;
		sMatrix4 m_view;
		sMatrix4 m_proj;
		sMatrix4 m_vp;

		sVector3 m_eye;
		sVector3 m_at;
		sVector3 m_up;

		cFrustum*		m_frustum;
	};
}











#endif













