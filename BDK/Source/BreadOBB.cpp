
#include "BreadOBB.h"


namespace Bread
{
	/*
	 *	
	 */
	void cOBB::set(const sVector3& center, const sMatrix3& rotation, const sVector3& extent)
	{
		m_center = center;
		m_rotation = rotation;
		m_extent = extent;
	}
	/*
	 *	
	 */
}