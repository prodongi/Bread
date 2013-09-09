
#ifdef _D3D9_

#include "BreadD3DSystem.h"
#include "BreadDeclaration.h"
#include "BreadUtils.h"
#include "BreadEngineConfig.h"
#include <tchar.h>

namespace Bread
{
	LPDIRECT3DDEVICE9 cD3DSystem::m_D3DDevice = NULL;

	/**
	 *	
	 */
	cVideoDriver::~cVideoDriver()
	{
		m_videoModeList.clear();
	}
	/**
	 *	
	 */
	void cVideoDriver::enumVideoModeList(LPDIRECT3D9 d3d)
	{
		enumVideoModeList(d3d, D3DFMT_R5G6B5);
		enumVideoModeList(d3d, D3DFMT_X8R8G8B8);
	}
	/**
	 *	
	 */
	void cVideoDriver::enumVideoModeList(LPDIRECT3D9 d3d, D3DFORMAT format)
	{
		UINT mode;
		UINT modeCount;

		modeCount = d3d->GetAdapterModeCount(m_adapterNum, format);

		for (mode = 0; mode < modeCount; ++mode)
		{
			D3DDISPLAYMODE displayMode;
			d3d->EnumAdapterModes(m_adapterNum, format, mode, &displayMode);

			// Filter out low-resolutions
			//if(displayMode.Width < 640 || displayMode.Height < 400)
			//	continue;

			if(displayMode.Width < 800 || displayMode.Height < 600)
				continue;

			// Check to see if it is already in the list (to filter out refresh rates)
			BOOL found = FALSE;
			std::vector<cVideoMode>::iterator it = m_videoModeList.begin();
			for(; it != m_videoModeList.end(); ++it)
			{
				if(it->m_displayMode.Width == displayMode.Width &&
					it->m_displayMode.Height == displayMode.Height &&
					it->m_displayMode.Format == displayMode.Format )
				{
					// Check refresh rate and favour higher if poss
					if (it->m_displayMode.RefreshRate < displayMode.RefreshRate)
						it->m_displayMode.RefreshRate = displayMode.RefreshRate;
					found = TRUE;
					break;
				}
			}

			if( !found )
			{
				cVideoMode vm;
				vm.m_displayMode = displayMode;
				m_videoModeList.push_back(vm);
			}
		}
	}


	/*
	 *	
	 */
	void sRttDeviceInfo::checkMSFloat16(LPDIRECT3D9 d3d, bool fullscreen)
	{
		m_enableMSFloat16 = false;

		BOOL window = (fullscreen) ? FALSE : TRUE;
		D3DCAPS9 caps;
		LPDIRECT3DDEVICE9 device = cD3DSystem::getD3DDevice();
		device->GetDeviceCaps(&caps);

		DWORD msQuaility = 0;
		D3DMULTISAMPLE_TYPE msType = D3DMULTISAMPLE_2_SAMPLES;
		for (; msType < D3DMULTISAMPLE_16_SAMPLES; msType = (D3DMULTISAMPLE_TYPE)(msType + 1))
		{
			if (SUCCEEDED(d3d->CheckDeviceMultiSampleType(caps.AdapterOrdinal, caps.DeviceType,
												D3DFMT_A16B16G16R16F, window, msType, &msQuaility)))
			{
				m_enableMSFloat16 = true;
				if (msQuaility > 0)	msQuaility -= 1;

				m_float16List.insert((uint)msType, sRttMS((uint)msType, (uint)msQuaility));
			}
		}
	}
	/*
	 *	
	 */
	bool sRttDeviceInfo::getFloat16Info(uint msType, uint& _msType, uint& _msQuaility)
	{
		if (!m_enableMSFloat16)
			return false;

		sRttMS rttMs;
		if (!m_float16List.get(msType, rttMs))
			return false;
		_msType = rttMs.m_type;
		_msQuaility = rttMs.m_quality;

		return true;
	}



	

	/**
	 *	
	 */
	cD3DSystem::cD3DSystem() : m_hInstance(NULL),
							   m_hWnd(NULL),
							   m_D3D(NULL),
							   m_renderSurface(NULL),
							   m_renderZBuffer(NULL),
							   m_clearColor(0xff333333)
	{
	}
	/**
	 *	
	 */
	cD3DSystem::~cD3DSystem()
	{
		finalize();		
	}
	/**
	 *	
	 */
	void cD3DSystem::initialize(HINSTANCE hInstance, WNDPROC wndProc, cEngineConfig const* config)
	{
		m_hInstance = hInstance;

		//Create Direct Object
		m_D3D = Direct3DCreate9(D3D_SDK_VERSION);
		if (!m_D3D)
			throw "error";
		
		enumDriver();
		createWindow(wndProc, config);
		createD3D(config);
		m_rttDeviceInfo.checkMSFloat16(m_D3D, config->getFullScreen());
	}
	/*
	 *	
	 */
	bool cD3DSystem::getRttMSFloat16Info(uint msType, uint& _msType, uint& _msQuaility)
	{
		return m_rttDeviceInfo.getFloat16Info(msType, _msType, _msQuaility);
	}
	/**
	 *	
	 */
	void cD3DSystem::enumDriver()
	{
		UINT adapterCount = m_D3D->GetAdapterCount();
		for (UINT i = 0; i < adapterCount; ++i)
		{
			cVideoDriver vd;
			vd.m_adapterNum = (int)i;
			m_D3D->GetAdapterIdentifier(i, 0, &vd.m_adapter);
			m_D3D->GetAdapterDisplayMode(i, &vd.m_displayMode);
			vd.enumVideoModeList(m_D3D);

			m_driverList.push_back(vd);
		}
	}
	/**
	 *	
	 */
	void cD3DSystem::createWindow(WNDPROC wndProc, cEngineConfig const* config)
	{
		cVideoDriver* activeDriver = &m_driverList.front();
		//cVideoMode* activeMode = NULL;

		int modeCount = (int)activeDriver->m_videoModeList.size();
		for (int i = 0; i < modeCount; ++i)
		{
			cVideoMode* mode = &activeDriver->m_videoModeList[i];

			D3DFORMAT format = mode->m_displayMode.Format; 
			int colorDepth = 16;
			if (format == D3DFMT_X8R8G8B8 || format == D3DFMT_A8R8G8B8 || format == D3DFMT_R8G8B8)
				colorDepth = 32;

			if (mode->m_displayMode.Width == (UINT)config->getWidth() &&
				mode->m_displayMode.Height == (UINT)config->getHeight() &&
				colorDepth == config->getColorDepth())
			{
				//activeMode = mode;
				break;
			}
		}


		int top=-1, left=-1;
		int width = config->getWidth();
		int height = config->getHeight();
		DWORD style = WS_VISIBLE | WS_CLIPCHILDREN;

		if (!config->getFullScreen())
		{
			style |= WS_OVERLAPPEDWINDOW;

			RECT rc;
			SetRect(&rc, 0, 0, width, height);
			AdjustWindowRect(&rc, style, false);
			width = rc.right - rc.left;
			height = rc.bottom - rc.top;

			// Clamp width and height to the desktop dimensions
			int screenW = GetSystemMetrics(SM_CXSCREEN);
			int screenH = GetSystemMetrics(SM_CYSCREEN);
			if (width > screenW)
				width = screenW;
			if (height > screenH)
				height = screenH;
			if (left < 0)
				left = (screenW - width) / 2;
			if (top < 0)
				top = (screenH - height) / 2;
		}
		else
		{
			style |= WS_POPUP;
			top = left = 0;
		}

		TCHAR const* className = _T("BreadD3D");
		// Register the window class
		// NB allow 4 bytes of window data for D3D9RenderWindow pointer
		WNDCLASS wc = { 0, wndProc, 0, 0, m_hInstance,
			LoadIcon(0, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)GetStockObject(BLACK_BRUSH), 0, className };
		RegisterClass(&wc);

		m_hWnd = CreateWindow(className, config->getTitle(), style,
			left, top, width, height, NULL, 0, m_hInstance, NULL);

		ShowWindow(m_hWnd, SW_SHOW);
	}
	/**
	 *	
	 */
	void cD3DSystem::createD3D(cEngineConfig const* config)
	{
		cVideoDriver* activeDriver = &m_driverList.front();

		D3DDEVTYPE devType = D3DDEVTYPE_HAL;

		ZeroMemory(&m_D3Dpp, sizeof(D3DPRESENT_PARAMETERS));
		m_D3Dpp.Windowed				= !config->getFullScreen();
		m_D3Dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
		// triple buffer if VSync is on
		m_D3Dpp.BackBufferCount			= config->getVSync() ? 2 : 1;
		m_D3Dpp.EnableAutoDepthStencil	= config->getDepthBuffer();
		m_D3Dpp.hDeviceWindow			= m_hWnd;
		m_D3Dpp.BackBufferWidth			= config->getWidth();
		m_D3Dpp.BackBufferHeight		= config->getHeight();
		m_D3Dpp.FullScreen_RefreshRateInHz = config->getFullScreen() ? config->getFrequency() : 0;
		m_D3Dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

		if (config->getVSync())
		{
			m_D3Dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
			if (config->getFullScreen())
				trace(_T("recommand vSync false when fullScreen is false\n"));
		}
		else
		{
			// NB not using vsync in windowed mode in D3D9 can cause jerking at low 
			// frame rates no matter what buffering modes are used (odd - perhaps a
			// timer issue in D3D9 since GL doesn't suffer from this) 
			// low is < 200fps in this context
			if (!config->getFullScreen())
			{
				//				LogManager::getSingleton().logMessage("D3D9 : WARNING - "
				//					"disabling VSync in windowed mode can cause timing issues at lower "
				//					"frame rates, turn VSync on if you observe this problem.");
			}
			m_D3Dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		}


		if (config->getColorDepth() < 16)
			m_D3Dpp.BackBufferFormat	= D3DFMT_R5G6B5;
		else
			m_D3Dpp.BackBufferFormat = D3DFMT_X8R8G8B8;


		if (config->getColorDepth() > 16 )
		{
			// Try to create a 32-bit depth, 8-bit stencil
			if (FAILED(m_D3D->CheckDeviceFormat(activeDriver->m_adapterNum, 
				devType,  m_D3Dpp.BackBufferFormat,  D3DUSAGE_DEPTHSTENCIL, 
				D3DRTYPE_SURFACE, D3DFMT_D24S8)))
			{
				// Bugger, no 8-bit hardware stencil, just try 32-bit zbuffer 
				if (FAILED( m_D3D->CheckDeviceFormat(activeDriver->m_adapterNum, 
					devType,  m_D3Dpp.BackBufferFormat, D3DUSAGE_DEPTHSTENCIL, 
					D3DRTYPE_SURFACE, D3DFMT_D32 )))
				{
					// Jeez, what a naff card. Fall back on 16-bit depth buffering
					m_D3Dpp.AutoDepthStencilFormat = D3DFMT_D16;
				}
				else
				{
					m_D3Dpp.AutoDepthStencilFormat = D3DFMT_D32;
				}
			}
			else
			{
				// Woohoo!
				if (SUCCEEDED(m_D3D->CheckDepthStencilMatch(activeDriver->m_adapterNum, devType,
					m_D3Dpp.BackBufferFormat, m_D3Dpp.BackBufferFormat, D3DFMT_D24S8)))
				{
					m_D3Dpp.AutoDepthStencilFormat = D3DFMT_D24S8; 
				} 
				else 
				{
					m_D3Dpp.AutoDepthStencilFormat = D3DFMT_D24X8; 
				}
			}
		}
		else
		{
			// 16-bit depth, software stencil
			m_D3Dpp.AutoDepthStencilFormat	= D3DFMT_D16;
		}

		m_D3Dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)config->getMultiSampleType();
		m_D3Dpp.MultiSampleQuality = config->getMultiSampleQuality();


		// We haven't created the device yet, this must be the first time

		// Do we want to preserve the FPU mode? Might be useful for scientific apps
		DWORD extraFlags = 0;
		extraFlags |= D3DCREATE_FPU_PRESERVE;
		extraFlags |= D3DCREATE_MULTITHREADED;

		// Set default settings (use the one Ogre discovered as a default)
		UINT adapterToUse = activeDriver->m_adapterNum;

		if (config->getUseHUD())
		{
			// Look for 'NVIDIA NVPerfHUD' adapter (<= v4)
			// or 'NVIDIA PerfHUD' (v5)
			// If it is present, override default settings
			for (UINT adapter=0; adapter < m_D3D->GetAdapterCount(); ++adapter)
			{
				D3DADAPTER_IDENTIFIER9 identifier;
				m_D3D->GetAdapterIdentifier(adapter,0, &identifier);
				if (strstr(identifier.Description,"PerfHUD") != 0)
				{
					adapterToUse = adapter;
					devType = D3DDEVTYPE_REF;
					break;
				}
			}
		}

		HRESULT hr;

		hr = m_D3D->CreateDevice(adapterToUse, devType, m_hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING | extraFlags, &m_D3Dpp, &m_D3DDevice);
		if (FAILED(hr))
		{
			// Try a second time, may fail the first time due to back buffer count,
			// which will be corrected down to 1 by the runtime
			hr = m_D3D->CreateDevice( adapterToUse, devType, m_hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | extraFlags, &m_D3Dpp, &m_D3DDevice );
		}
		if (FAILED(hr))
		{
			hr = m_D3D->CreateDevice( adapterToUse, devType, m_hWnd,
				D3DCREATE_MIXED_VERTEXPROCESSING | extraFlags, &m_D3Dpp, &m_D3DDevice );
			if( FAILED( hr ) )
			{
				hr = m_D3D->CreateDevice( adapterToUse, devType, m_hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING | extraFlags, &m_D3Dpp, &m_D3DDevice );
			}
		}
		// TODO: make this a bit better e.g. go from pure vertex processing to software
		if (FAILED(hr))
		{
			throw "Failed to create Direct3D9 Device";
		}

		// Store references to buffers for convenience
		m_D3DDevice->GetRenderTarget(0, &m_renderSurface);
		m_D3DDevice->GetDepthStencilSurface(&m_renderZBuffer);
		// release immediately so we don't hog them
		m_renderZBuffer->Release();

		checkDevice();
	}
	/*
	 *	
	 */
	void cD3DSystem::checkDevice()
	{
		m_checkDevice.query(m_D3D, m_D3DDevice, m_driverList[0].m_displayMode.Format, m_D3Dpp.BackBufferFormat);
	}
	/**
	 *	
	 */
	void cD3DSystem::finalize()
	{
		m_driverList.clear();

		safeRelease(m_renderSurface);
		safeRelease(m_renderZBuffer);
		safeRelease(m_D3DDevice);
		safeRelease(m_D3D);
	}
}

#endif