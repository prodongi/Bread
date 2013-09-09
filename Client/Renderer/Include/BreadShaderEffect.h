
#ifndef _BreadShaderEffect_h_
#define _BreadShaderEffect_h_

/*
	BreadShaderEffect.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-yy-dd
	purpose : 
*/

#include "BreadDeclaration.h"
#include "BreadD3DHeader.h"

namespace Bread
{
	class cTexture;
	struct sMatrix4;
	struct sVector4;

	/*
	*/
	class cShaderEffect
	{
	public:
		cShaderEffect();
		~cShaderEffect();
		bool createFromFile(LPDIRECT3DDEVICE9 device, std_string const& filename, DWORD flags, LPD3DXEFFECTPOOL pool);
		bool create(LPDIRECT3DDEVICE9 device, char const* srcData, uint srcDataLen, DWORD flags, LPD3DXEFFECTPOOL pool);
		void begin(uint* pass);
		void end();
		void beginPass(uint p);
		void endPass();
		void lostDevice();
		void resetDevice();
		D3DXHANDLE getTechniqueByName(TCHAR const* name);
		D3DXHANDLE getParameterByName(TCHAR const* name);
		void setTechnique(D3DXHANDLE handle);
		void setMatrix(D3DXHANDLE handle, D3DXMATRIXA16 const* mat);
		void setMatrix(D3DXHANDLE handle, sMatrix4 const* mat);
		void setMatrixArray(D3DXHANDLE handle, D3DXMATRIXA16 const* mat, uint num);
		void setMatrixArray(D3DXHANDLE handle, sMatrix4 const* mat, uint num);
		void setVector(D3DXHANDLE handle, D3DXVECTOR4 const* vec);
		void setVector(D3DXHANDLE handle, sVector4 const* vec);
		void setVectorArray(D3DXHANDLE handle, D3DXVECTOR4 const* vec, uint num);
		void setFloat(D3DXHANDLE handle, float f);
		void setFloatArray(D3DXHANDLE handle, float const* f, uint num);
		void setInt(D3DXHANDLE handle, int i);
		void setIntArray(D3DXHANDLE handle, int const* i, uint num);
		void setBool(D3DXHANDLE handle, BOOL b);
		void setValue(D3DXHANDLE handle, void* value, int size);
		void setTexture(D3DXHANDLE handle, cTexture const*tex);

	private:
		ID3DXEffect* m_effect;
	};
}

#endif
