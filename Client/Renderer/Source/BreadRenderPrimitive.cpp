
#include "BreadRenderPrimitive.h"


namespace Bread
{
	DWORD sPointVertex::D3DFVF = D3DFVF_XYZ;
	size_t sPointVertex::SIZE = sizeof (sPointVertex);

	DWORD sLineVertex::D3DFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	size_t sLineVertex::SIZE = sizeof (sLineVertex);

	DWORD sUvVertex::D3DFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	size_t sUvVertex::SIZE = sizeof (sUvVertex);

	DWORD sUvColorVertex::D3DFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	size_t sUvColorVertex::SIZE = sizeof (sUvColorVertex);

	DWORD sFaceVertex::D3DFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	size_t sFaceVertex::SIZE = sizeof (sFaceVertex);

	DWORD sVolumeCloudVertex::D3DFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	size_t sVolumeCloudVertex::SIZE = sizeof (sVolumeCloudVertex);
}