
#ifndef _BreadD3DHeader_h_
#define _BreadD3DHeader_h_

/*
	DirectX SDK Version : June 2010
*/

#pragma warning(disable : 4324)

#ifdef _D3D9_

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>

#elif _D3D11_

#include <d3d11.h>
#include <d3dx11.h>

#else

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>

#endif

#include <dxerr.h>

#endif