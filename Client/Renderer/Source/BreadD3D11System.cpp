
#ifdef _D3D11_

#include "BreadD3D11System.h"

namespace Bread
{
	/*
	*/
	cD3DSystem::cD3DSystem() : m_swapChain(NULL),
							   m_immediateContext(NULL),
							   m_d3dDevice(NULL),
							   m_renderTargetView(NULL);
	{
	}
	/*
	*/
	cD3DSystem::~cD3DSystem()
	{
	}
	/*
	*/
	void cD3DSystem::initialize(HINSTANCE hInstance, WNDPROC wndProc, cEngineConfig const* config)
	{
		m_hInstance = hInstance;

		enumDriver();
		createWindow(wndProc, config);
		createD3D(config);
	}
	/*
	*/
	void cD3DSystem::enumDriver()
	{
		/*
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
		*/
	}
	/*
	*/
	void cD3DSystem::createWindow(WNDPROC wndProc, cEngineConfig const* config)
	{
		/*
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

		*/
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
	/*
	*/
	void cD3DSystem::createD3D(cEngineConfig const* config)
	{
		uint deviceFlags = 0;
#ifdef _DEBUG
		deviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] = 
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		uint numDriverTypes = ARRAYSIZE(driverType);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		uint numFeatureLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof (DXGI_SWAP_CHAIN_DESC));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = config->getWidth();
		sd.BufferDesc.Height = config->getHeight();
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = m_hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = !config->getFullScreen();

		HRESULT hr;
		D3D_DRIVER_TYPE driverType;
		for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
		{
			driverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain( NULL, driverType, NULL, deviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &sd, &m_swapChain, &m_d3dDevice, &m_featureLevel, &m_immediateContext );
			if( SUCCEEDED( hr ) )
				break;
		}
		if( FAILED( hr ) )
			return hr;

		ID3D11Texture2D* backBuffer = NULL;
		hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		if (FAILED(hr))
			return hr;

		hr = m_d3dDevice->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
		if (FAILED(hr))
			return hr;

		m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);

		D3D11_VIEWPORT vp;
		vp.Width = (float)config->getWidth();
		vp.Height = (float)config->getHeight();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_immediateContext->RSSetViewports(1, &vp);

		return S_OK;
	}
	/*
	*/
	void cD3DSystem::finalize()
	{
		if (m_immediateContext)
			m_immediateContext->ClearState();

		safeRelease(m_renderTargetView);
		safeRelease(m_swapChain);
		safeRelease(m_immediateContext);
		safeRelease(m_d3dDevice);
	}
}

#endif