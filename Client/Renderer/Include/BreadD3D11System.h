
#ifndef _BreadD3D11System_h_
#define _BreadD3D11System_h_

#ifdef _D3D11_

#include "BreadDeclaration.h"
#include "BreadHelperClass.h"

namespace Bread
{
	class cEngineConfig;

	/**
	 * @class cVideoDirver
	 *
	 * @date 2008-09-17
	 */
	/*
	class cVideoDriver
	{
	public:
		cVideoDriver() {}
		~cVideoDriver();

		void enumVideoModeList(LPDIRECT3D9 d3d);
		void enumVideoModeList(LPDIRECT3D9 d3d, D3DFORMAT format); 

		int m_adapterNum;
		D3DADAPTER_IDENTIFIER9 m_adapter;
		D3DDISPLAYMODE m_displayMode;

		std::vector<cVideoMode>	m_videoModeList;
	};
	*/
	/*
	*/
	class cD3DSystem
	{
	public:
		cD3DSystem();
		~cD3DSystem();
		void initialize(HINSTANCE hInstance, WNDPROC wndProc, cEngineConfig const* config);

	private:
		void enumDriver();
		void createWindow(WNDPROC wndProc, cEngineConfig const* config);
		void createD3D(cEngineConfig const* config);
		void finalize();

	private:
		HWND m_hWnd;
		HINSTANCE m_hInstance;
		DWORD m_clearColor;
		IDXGISwapChain* m_swapChain;
		D3D_FEATURE_LEVEL m_featureLevel;
		ID3D11DeviceContext* m_immediateContext;
		ID3D11Device* m_d3dDevice;
		ID3D11RenderTargetView* m_renderTargetView;
		//std::vector<cVideoDriver> m_driverList;
	};
}

#endif

#endif