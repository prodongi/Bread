
#include "BreadDebug.h"
#include "BreadUtils.h"
#include <assert.h>

namespace Bread
{
	/*
	*/
	bool checkHResultReturn(HRESULT hr)
	{
		if (FAILED(hr))
		{
			trace(_T("Failed HResult : %s\n"), DXGetErrorString(hr));
			assert(0 && _T("Failed HResult"));
			return false;
		}
		return true;
	}
	/*
	*/
	void checkHResult(HRESULT hr)
	{
		if (FAILED(hr))
		{
			trace(_T("Failed HResult : %s\n"), DXGetErrorString(hr));
			assert(0 && _T("Failed HResult"));
			return ;
		}
	}
	/*
	 *	
	 */
	void enableFPExeptions()
	{
		int i = _controlfp(0, 0);
		i &= ~(EM_ZERODIVIDE|EM_OVERFLOW|EM_INVALID);
		_controlfp(i, MCW_EM);
	}
	/*
	*/
	cRenderChannel::sChannel cRenderChannel::m_channel[] = {cRenderChannel::sChannel()};
	/*
	*/
	void cRenderChannel::initialize()
	{
		_setIsRender(RENDER_SCENE, true);
		_setIsRender(RENDER_SCENE_OCTREE, false);
		_setIsRender(RENDER_MESH_CULLFIGURE, false);
		_setIsRender(RENDER_BONE, false);
		_setIsRender(RENDER_GRID, false);
		_setIsRender(RENDER_TERRAIN, true);
		_setIsRender(RENDER_PICK, false);
		_setIsRender(RENDER_WIRE, false);
		_setIsRender(RENDER_NORMAL, false);
		_setIsRender(RENDER_TANGENT, false);
		_setIsRender(RENDER_BINORMAL, false);
		_setIsRender(RENDER_RENDERLIST_CULLFIGURE, false);
		_setIsRender(RENDER_PROFILE, false);
		_setIsRender(RENDER_DEBUG_INFO, false);
		_setIsRender(RENDER_FPS, false);
		_setIsRender(RENDER_MOVE_PROXY_PATH, false);
		
		_setIsRenderValue1(RENDER_SCENE_OCTREE, 4);
		_setIsRenderValue1(RENDER_PROFILE, 800);
		_setIsRenderValue2(RENDER_PROFILE, 30);
	}
}
