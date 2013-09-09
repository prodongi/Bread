
#ifndef _BreadBoundingSphere_h_
#define _BreadBoundingSphere_h_


#include "BreadVector3.h"

class sLine;

namespace Bread
{
	/**
	 * \ingroup Core
	 *
	 * \class cBoundingSphere
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2008-09-30
	 *
	 * \author Administrator
	 * 
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	class cBoundingSphere
	{
	public:
		cBoundingSphere()	{}
		cBoundingSphere(const sVector3& center, float radius) : m_center(center), m_radius(radius)	{}
		~cBoundingSphere();

		void set(const sVector3* vecList, unsigned int vertNum);
		bool collision(const cBoundingSphere& sphere);
		bool collision(const sLine& line);

	public:
		sVector3 m_center;
		float	 m_radius;
	};
}














#endif










