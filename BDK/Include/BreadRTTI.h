
#ifndef _BreadRTTI_h_
#define _BreadRTTI_h_

#include "BreadStdHeader.h"

namespace Bread
{
	/*
		@date 2012.01.28
		@auth prodongi
		@desc 
		@todo 
	*/
	class cRTTI
	{
	public:
		cRTTI(std::string const& className) : m_className(className), m_parentRTTI(NULL) {};
		cRTTI(std::string const& className, cRTTI const* parentRTTI) : m_className(className), m_parentRTTI(parentRTTI) {}
		std::string const& getClassName() const	{	return m_className;	}
		cRTTI const* getParentRTTI() const			{	return m_parentRTTI;	}

	private:
		std::string m_className;
		cRTTI const* m_parentRTTI;
	};
}



#endif