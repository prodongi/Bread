
#include "BreadUiPrimitive.h"


namespace Bread
{
	DWORD sUiVertex::D3DFVF			= D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	size_t sUiVertex::SIZE			= sizeof (sUiVertex);

	DWORD sUiPlane::D3DFVF			= sUiVertex::D3DFVF;
	size_t sUiPlane::VERTEX_SIZE	= sUiVertex::SIZE;

	DWORD sUiT4Vertex::D3DFVF		= D3DFVF_XYZRHW | D3DFVF_TEX4;
	size_t sUiT4Vertex::SIZE		= sizeof (sUiT4Vertex);

	DWORD sUiT4Plane::D3DFVF		= sUiT4Vertex::D3DFVF;
	size_t sUiT4Plane::VERTEX_SIZE	= sUiVertex::SIZE;
}