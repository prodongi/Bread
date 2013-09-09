
#ifndef _BreadAABB_h_
#define _BreadAABB_h_

#include "BreadVector3.h"


namespace Bread
{
	struct sRay;
	struct sPlane;

	/**
	 * \ingroup Core
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
	 * \date 2008-10-09
	 *
	 * \author Administrator
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
	class cAABB
	{
	public:
		cAABB(){}
		cAABB(const sVector3& min, const sVector3& max)
		{
			m_min = min;
			m_max = max;
		}
		~cAABB(){}

		void set(const sVector3& min, const sVector3& max);
		bool collision(const cAABB& other) const;
		bool collision(const sRay& ray) const;
		float collision(const sPlane& plane) const;

	public:
		sVector3 m_min;
		sVector3 m_max;
	};
}







#endif











