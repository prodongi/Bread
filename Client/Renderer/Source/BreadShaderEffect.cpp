
/*
	BreadShaderEffect.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "BreadShaderEffect.h"
#include "BreadFileSystem.h"
#include "BreadStringHelper.h"
#include "BreadTexture.h"
#include "BreadMatrix4.h"
#include "BreadVector4.h"

namespace Bread
{
	/*
	*/
	cShaderEffect::cShaderEffect() : m_effect(NULL)
	{
	}
	/*
	*/
	cShaderEffect::~cShaderEffect()
	{
		safeRelease(m_effect);
	}
	/*
	*/
	bool cShaderEffect::createFromFile(LPDIRECT3DDEVICE9 device, std_string const& filename, DWORD flags, LPD3DXEFFECTPOOL pool)
	{
		std_string realFileName;
		if (!_getFileSystem()->findRealPath(filename, realFileName))
		{
			assert(0 && _T("failed find file at createEffect"));
			return false;
		}

		HRESULT hr = D3DXCreateEffectFromFile(device, realFileName.c_str(), NULL, NULL, flags, pool, &m_effect, NULL);
		if (!checkHResultReturn(hr))
			return false;

		return true;
	}
	/*
	*/
	bool cShaderEffect::create(LPDIRECT3DDEVICE9 device, char const* srcData, uint srcDataLen, DWORD flags, LPD3DXEFFECTPOOL pool)
	{
		HRESULT hr = D3DXCreateEffect(device, (void*)srcData, srcDataLen, NULL, NULL, flags, pool, &m_effect, NULL);
		if (!checkHResultReturn(hr))
			return false;

		return true;
	}
	void cShaderEffect::begin(uint* pass)		{	m_effect->Begin(pass, 0);	}
	void cShaderEffect::end()					{	m_effect->End();			}
	void cShaderEffect::beginPass(uint p)		{	m_effect->BeginPass(p);		}
	void cShaderEffect::endPass()				{	m_effect->EndPass();		}
	void cShaderEffect::lostDevice()			{	m_effect->OnLostDevice();	}
	void cShaderEffect::resetDevice()			{	m_effect->OnResetDevice();	}
	/*
	*/
	D3DXHANDLE cShaderEffect::getTechniqueByName(TCHAR const* name)
	{
		if (checkIsNull(m_effect))
			return NULL;

		char str[NAME_LEN];
		_getStringHelper()->w2m(name, str, NAME_LEN);

		D3DXHANDLE handle = m_effect->GetTechniqueByName(str);
		if (!handle)
		{
			assert(0 && _T("Failed insert technque"));
			return NULL;
		}
		return handle;
	}
	/*
	*/
	D3DXHANDLE cShaderEffect::getParameterByName(TCHAR const* name)
	{
		if (checkIsNull(m_effect))
			return NULL;

		char str[NAME_LEN];
		_getStringHelper()->w2m(name, str, NAME_LEN);

		D3DXHANDLE handle = m_effect->GetParameterByName(NULL, str);
		if (!handle)
		{
			assert(0 && _T("Failed insert parameter"));
			return NULL;
		}
		return handle;
	}
	/*
	*/
	void cShaderEffect::setTechnique(D3DXHANDLE handle)
	{
		if (checkIsNull(m_effect))
			return ;
		if (!handle || FAILED(m_effect->SetTechnique(handle)))
		{
			assert(0 && _T("Failed set technique"));
			return ;
		}
	}
	/*
	*/
	void cShaderEffect::setMatrix(D3DXHANDLE handle, D3DXMATRIXA16 const* mat)
	{
		if (checkIsNull(m_effect))
			return ;
		if (!handle || FAILED(m_effect->SetMatrix(handle, mat)))
		{
			assert(0 && _T("Failed set matrix"));
			return ;
		}
	}
	/*
	*/
	void cShaderEffect::setMatrix(D3DXHANDLE handle, sMatrix4 const* mat)
	{
		static D3DXMATRIXA16 _mat;
		mat->to(_mat);
		setMatrix(handle, &_mat);
	}
	/*
	*/
	void cShaderEffect::setMatrixArray(D3DXHANDLE handle, D3DXMATRIXA16 const* mat, uint num)
	{
		if (checkIsNull(m_effect))
			return ;
		if (!handle || FAILED(m_effect->SetMatrixArray(handle, mat, num)))
		{
			assert(0 && _T("Failed set matrix"));
			return ;
		}
	}
	/*
	*/
	void cShaderEffect::setMatrixArray(D3DXHANDLE handle, sMatrix4 const* mat, uint num)
	{
		static D3DXMATRIXA16 _mat[4];
		assert(num <= 4 && _T("matrix array static num is over"));
		for (uint i = 0; i < num; ++i)
			mat[i].to(_mat[i]);
		setMatrixArray(handle, _mat, num);
	}
	/*
	*/
	void cShaderEffect::setVector(D3DXHANDLE handle, D3DXVECTOR4 const* vec)
	{
		if (checkIsNull(m_effect))
			return ;
		if (!handle || FAILED(m_effect->SetVector(handle, vec)))
		{
			assert(0 && _T("Failed set vector"));
			return ;
		}
	}
	/*
	*/
	void cShaderEffect::setVector(D3DXHANDLE handle, sVector4 const* vec)
	{
		if (checkIsNull(m_effect))
			return ;
		static D3DXVECTOR4 _vec;
		vec->to4(_vec);
		setVector(handle, &_vec);
	}
	/*
	*/
	void cShaderEffect::setVectorArray(D3DXHANDLE handle, D3DXVECTOR4 const* vec, uint num)
	{
		if (checkIsNull(m_effect))
			return ;
		if (!handle || FAILED(m_effect->SetVectorArray(handle, vec, num)))
		{
			assert(0 && _T("Failed set vector"));
			return ;
		}
	}
	/*
	*/
	void cShaderEffect::setFloat(D3DXHANDLE handle, float f)
	{
		if (checkIsNull(m_effect))
			return ;
		if (!handle || FAILED(m_effect->SetFloat(handle, f)))
		{
			assert(0 && _T("Failed set float"));
			return ;
		}
	}
	/*
	*/
	void cShaderEffect::setFloatArray(D3DXHANDLE handle, float const* f, uint num)
	{
		if (checkIsNull(m_effect))
			return ;
		if (!handle || FAILED(m_effect->SetFloatArray(handle, f, num)))
		{
			assert(0 && _T("Failed set float array"));
			return ;
		}
	}
	/*
	*/
	void cShaderEffect::setInt(D3DXHANDLE handle, int i)
	{
		if (checkIsNull(m_effect))
			return ;
		if (!handle || FAILED(m_effect->SetInt(handle, i)))
		{
			assert(0 && _T("Failed set int"));
			return ;
		}
	}
	/*
	*/
	void cShaderEffect::setIntArray(D3DXHANDLE handle, int const* i, uint num)
	{
		if (checkIsNull(m_effect))
			return ;
		if (!handle || FAILED(m_effect->SetIntArray(handle, i, num)))
		{
			assert(0 && _T("Failed set int array"));
			return ;
		}
	}
	/*
	*/
	void cShaderEffect::setBool(D3DXHANDLE handle, BOOL b)
	{
		if (checkIsNull(m_effect))
			return ;
		if (!handle || FAILED(m_effect->SetBool(handle, b)))
		{
			assert(0 && _T("Failed set int array"));
			return ;
		}
	}
	/*
	*/
	void cShaderEffect::setValue(D3DXHANDLE handle, void* value, int size)
	{
		if (checkIsNull(m_effect))
			return ;
		if (!handle || FAILED(m_effect->SetValue(handle, value, size)))
		{
			assert(0 && _T("Failed set value"));
			return ;
		}
	}
	/*
	*/
	void cShaderEffect::setTexture(D3DXHANDLE handle, cTexture const*tex)
	{
		if (checkIsNull(m_effect))
			return ;
		if (!tex)
			return ;

		if (!handle || FAILED(m_effect->SetTexture(handle, tex->getTexture())))
		{
			assert(0 && _T("Failed set texture"));
			return ;
		}
	}
}