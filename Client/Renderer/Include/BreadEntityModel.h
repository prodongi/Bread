
#ifndef _BreadEntityModel_h_
#define _BreadEntityModel_h_

#include "BreadD3DHeader.h"
#include "BreadEntity.h"

namespace Bread
{
	class cShader;
	class cMesh;
	class cAnimationAction;

	/**
	 * \ingroup Renderer
	 *
	 *
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2009-09-23
	 *
	 * \author Prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	class cEntityModel : public cEntity
	{
		declareRTTI()

	public:
		cEntityModel();
		virtual ~cEntityModel();
		virtual void initialize(BUID const& buid);
		virtual void initialize(sVector3 const& pos, float radian, BUID const& renderDataBuid, BUID const& animationBuid, bool isSceneObject, bool isSyncHeight = true);
		virtual void initialize(BUID const& buid, sVector3 const& pos, float radian, BUID const& renderDataBuid, BUID const& animationBuid, bool isSceneObject, bool isSyncHeight = true);
		virtual void update(float /*elapsedtime*/);
		virtual void setPosition(sVector3 const& pos, bool isSyncHeight = true);
		virtual void addPosition(sVector3 const& offset, bool isSyncHeight = true);
		virtual void setRotation(float radian);
		virtual void setTranslation(sVector3 const& pos, float radian, bool isSyncHeight = true);
		virtual void setMotion(uint id);
		virtual void setMoveType(uint type)	{	m_moveType = type;	}
		virtual bool isDecal() const {	return false; }

		propertyConstFunc(BUID, RenderDataBuid, m_renderDataBuid)
		propertyConstFunc(BUID, AnimationBuid, m_animationBuid)
		propertyFunc(cAnimationAction*, AnimationAction, m_action)

	protected:
		BUID m_renderDataBuid;
		BUID m_animationBuid;
		cAnimationAction* m_action;
		uint m_moveType;
	};
}






#endif









