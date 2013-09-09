
#ifndef _BreadD3DSystem_h_
#define _BreadD3DSystem_h_

#ifdef _D3D9_

#include "BreadDeclaration.h"
#include "BreadHelperClass.h"
#include "BreadD3DCheckDevice.h"

namespace Bread
{
	class cEngineConfig;

	/*
	 *	
	 */
	class cVideoMode
	{
	public:
		D3DDISPLAYMODE	m_displayMode;
	};

	/**
	 * @class cVideoDirver
	 *
	 * @date 2008-09-17
	 */
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
	/*
	 *	
	 */
	struct sRttDeviceInfo
	{
		struct sRttMS
		{
			sRttMS() : m_type(0), m_quality(0)	{}
			sRttMS(uint msType, uint msQuaility) : m_type(msType), m_quality(msQuaility) {}
			uint m_type;
			uint m_quality;
		};
		sRttDeviceInfo() : m_enableMSFloat16(false) {};
		~sRttDeviceInfo() {};
		void checkMSFloat16(LPDIRECT3D9 d3d, bool fullscreen);
		bool getFloat16Info(uint msType, uint& _msType, uint& _msQuaility);

		bool m_enableMSFloat16;
		cStdMapHelper<uint, sRttMS, cStdMapHelperClear>	m_float16List;
	};
	/**
	 * @class cD3DSystem
	 *
	 * \date 2008-09-05
	 */
	class cD3DSystem
	{
	public:
		cD3DSystem();
		~cD3DSystem();

		void initialize(HINSTANCE hInstance, WNDPROC wndProc, cEngineConfig const* config);
		bool getRttMSFloat16Info(uint msType, uint& _msType, uint& _msQuaility);
		static LPDIRECT3DDEVICE9 getD3DDevice()	{	return m_D3DDevice;	}
		HWND getHwnd() const					{	return m_hWnd;		}
		D3DPRESENT_PARAMETERS const* getParameters() const	{	return &m_D3Dpp;	}
		sCheckDevice const* getCheckDevice() const	{	return &m_checkDevice;	}
		
		propertyConstFunc(DWORD, ClearColor, m_clearColor)

		friend class cRenderer;
	private:
		void enumDriver();
		void createWindow(WNDPROC wndProc, cEngineConfig const* config);
		void createD3D(cEngineConfig const* config);
		void checkDevice();
		void finalize();

	private:
		HWND m_hWnd;
		HINSTANCE m_hInstance;
		LPDIRECT3D9	m_D3D;
		static LPDIRECT3DDEVICE9 m_D3DDevice;
		D3DPRESENT_PARAMETERS m_D3Dpp;
		LPDIRECT3DSURFACE9 m_renderSurface;
		LPDIRECT3DSURFACE9 m_renderZBuffer;
		DWORD m_clearColor;
		std::vector<cVideoDriver> m_driverList;
		sRttDeviceInfo m_rttDeviceInfo;
		sCheckDevice m_checkDevice;
	};
}

#endif

#endif