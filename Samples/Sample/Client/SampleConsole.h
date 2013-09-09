
#ifndef _SampleConsole_h_
#define _SampleConsole_h_

#include "BreadConsole.h"

/*
*/
class cSampleConsole : public cConsole
{
public:
	cSampleConsole() {}
	virtual ~cSampleConsole() {}
	virtual void initialize(float x, float y, float w, float h);
	virtual void update(float elapsedtime, int x, int y);
};















#endif