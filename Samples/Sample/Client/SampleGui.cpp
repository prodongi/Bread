
#include "stdafx.h"
#include "SampleGui.h"
#include "SampleConsole.h"

/*
*/
cSampleGui::cSampleGui()
{
}
/*
*/
cSampleGui::~cSampleGui()
{
}
/*
*/
bool cSampleGui::initialize(LPDIRECT3DDEVICE9 device, HINSTANCE hInstance, HWND hWnd)
{
	if (!cGui::initialize(device, hInstance, hWnd))
		return false;

	return true;
}
/*
*/
void cSampleGui::initConsole()
{
	cSampleConsole* console = new cSampleConsole;
	console->initialize(0, 0, 1.0f, 1.0f);
	m_windowList.add(console);
}












