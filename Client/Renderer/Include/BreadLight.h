
#ifndef _BreadLight_h_
#define _BreadLight_h_

#include "BreadMatrix4.h"
#include "BreadDeclaration.h"

namespace Bread
{
	class cLight
	{
	public:
		cLight() : m_type(0) {}
		virtual ~cLight() {}
		virtual void update(float /*elapsedtime*/) {}
		void calculMatVP();
		/// @brief nvidia sdk 9.5 에서 참조함
		void determineScissorRect(RECT* rect) const;
		sMatrix4 const& getMatVP() const		{	return m_matVP;	}
		sMatrix4 const& getMatView() const		{	return m_matView;	}
		sMatrix4 const& getMatProj() const		{	return m_matProj;	}
		void setMatVP(sMatrix4 const& matVP)	{	m_matVP = matVP;	}
		bool isType(int type) const				{	return m_type == type;	}
		void setPosition(sVector3 const& pos);
		void setDirection(sVector3 const& dir);
		int renderDebugInfo(int y);
		sVector3 const& getPosition() const		{	return m_position;	}
		sVector3 const& getDirection() const	{	return m_direction;	}
		sVector3 const& getUp() const			{	return m_up;		}
		propertyConstFunc(int, Type, m_type)
		propertyConstFunc(sVector3, Color, m_color)
		propertyConstFunc(float, Range, m_range)
		propertyConstFunc(BUID, Entity, m_entity)
		propertyConstFunc(float, Theta, m_theta)
		propertyConstFunc(float, Phi, m_phi)

	protected:
		void syncEntityPos();

	protected:
		/// @brief D3DLIGHTTYPE
		int m_type;
		float m_range;
		float m_theta;
		float m_phi;
		sVector3 m_color;
		sVector3 m_direction;
		sVector3 m_position;
		sVector3 m_up;
		sMatrix4 m_matVP;
		sMatrix4 m_matView;
		sMatrix4 m_matProj;
		BUID m_entity;
	};
}
















#endif