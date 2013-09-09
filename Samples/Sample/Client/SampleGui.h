
#ifndef _SampleGui_h_
#define _SampleGui_h_

#include "BreadGui.h"

class cSampleGui : public cGui
{
public:
	cSampleGui();
	virtual ~cSampleGui();
	virtual bool initialize(LPDIRECT3DDEVICE9 device, HINSTANCE hInstance, HWND hWnd);
	virtual void initConsole();
};















#endif