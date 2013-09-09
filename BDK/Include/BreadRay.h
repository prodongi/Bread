
#ifndef _BreadRay_h_
#define _BreadRay_h_


#include "BreadVector3.h"

namespace Bread
{
	struct sRay
	{
	public:
		sRay() {};
		sRay(sVector3 const& orig, sVector3 const& dir) : m_orig(orig), m_dir(dir) {}
		~sRay() {};

	public:
		sVector3 m_orig;
		sVector3 m_dir;
	};
}



#endif

















