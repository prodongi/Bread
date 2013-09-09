
#include "BreadProfile.h"
#include "BreadProfileManager.h"
#include "BreadInternString.h"

namespace Bread
{
	/*
	*/
	cProfile::cProfile(TCHAR const* name)
	{
		cProfileManager::start(_getInternString()->getId(name));
	}
	/*
	*/
	cProfile::~cProfile()
	{
		cProfileManager::stop();
	}
}