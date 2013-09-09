
#ifndef _BreadEntity_h_
#define _BreadEntity_h_

#include "BreadDeclaration.h"
#include "BreadVector3.h"
#include "BreadMatrix4.h"
#include "BreadEntityProperty.h"

namespace Bread
{
	/*
	*/
	enum ENTITY_MOVE_TYPE {		ENTITY_MOVE_WALK,
								ENTITY_MOVE_RUN
						  };

	class cEntity
	{
		declareRTTI()

	public:
		cEntity();
		virtual ~cEntity() {}
		virtual void initialize(BUID const& buid);
		virtual void initialize(sVector3 const& pos, float radian, bool isSceneObject, bool isSyncHeight = true);
		virtual void initialize(BUID const& buid, sVector3 const& pos, float radian, bool isSceneObject, bool isSyncHeight = true);
		virtual void setMotion(uint /*id*/) {}
		virtual void setMoveType(uint /*type*/) {}
		/// @brief setRotation + setPosition
		virtual void setTranslation(sVector3 const& pos, float radian, bool isSyncHeight = true);
		void setRevisionRadian(float radian)			{	m_revisionRadian = radian;			}
		sMatrix4 const& getMatRot() const				{	return m_matRot;					}
		bool getIsSceneObject() const					{	return m_isSceneObject;				}
		uint getCellIndex() const						{	return m_cellIndex;					}
		void calcuMatWorld(sMatrix4& matWorld) const;
		bool isValidRenderMask(uint renderMask) const;
		void syncHeight();
		propertyConstFunc(float, FadeAlpha, m_fadeAlpha)
		propertyConstFunc(uint, RenderMask, m_renderMask)

		/*	
			property function
		*/
		virtual void setRotation(float radian);
		virtual void setPosition(sVector3 const& pos, bool isSyncHeight = true);
		virtual void addPosition(sVector3 const& offset, bool isSyncHeight = true);
		sVector3 const& getPosition() const				{	return m_property.m_position;		}
		float getRadian() const							{	return m_property.m_radian;			}
		BUID const& getBuid() const						{	return m_property.m_buid;			}
		float getSpeed() const							{	return m_property.m_speed;			}
		void setSpeed(float speed)						{	m_property.m_speed = speed;			}
		
	protected:
		/// @brief entity 보정 방향값, 실제 모델의 방향이 x축을 보고 있지 않을 수도 있기 때문에 보정 값을 둔다
		float getRevisionRadian() const				{	return m_revisionRadian;			}

	protected:
		/// @brief m_radian을 y축으로 회전한 값, 보정값도 적용이 되었다.
		sMatrix4 m_matRot;
		/// @brief 배경에 쓰이는 entity인지/아닌지
		bool m_isSceneObject;
		uint m_renderMask;

	private:
		/// @brief 각도 보정 값
		float m_revisionRadian;
		float m_fadeAlpha;
		/// @brief 지형 셀 인덱스
		uint m_cellIndex;
		sEntityProperty m_property;
	};
}















#endif













