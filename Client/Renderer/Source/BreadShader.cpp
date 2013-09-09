
#include "BreadShader.h"
#include "BreadUtils.h"
#include "BreadVector2.h"
#include "BreadVector3.h"
#include "BreadMatrix4.h"
#include "BreadStringHelper.h"
#include "BreadTexture.h"
#include "BreadFileSystem.h"
#include "BreadShaderEffect.h"

namespace Bread
{
	/*
	 *	
	 */
	DWORD cShader::makeFlags()
	{
		DWORD flags;
		flags = D3DXFX_NOT_CLONEABLE | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY;

#ifdef _DEBUG
		flags |= D3DXSHADER_DEBUG;
#endif

#ifdef _DEBUG_VS
		flags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
		flags |= D3DXSHADER_SKIPOPTIMIZATION;
#endif
#ifdef _DEBUG_PS
		flags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
		flags |= D3DXSHADER_SKIPOPTIMIZATION;
#endif
		return flags;
	}


	/*
	 *	
	 */
	TCHAR const* cShader::makeBUID(std_string const& filename)
	{
		return _getStringHelper()->makeFilenameToBUID(filename);
	}

	/*
	 *	
	 */
	cShader::cShader() : m_effect(NULL)
	{
	}
	/*
	 *	
	 */
	cShader::~cShader()
	{
		safeDelete(m_effect);
	}
	/*
	 *	
	 */
	bool cShader::createEffect(std_string const& filename, LPDIRECT3DDEVICE9 device, DWORD flags, LPD3DXEFFECTPOOL pool)
	{
		m_filename = filename;
		m_buid = cShader::makeBUID(filename);

		if (!m_effect->createFromFile(device, filename, flags, pool))
			return false;
		
		if (!initialize())
			return false;
		registHandle();

		return true;
	}
	/*
	 *	
	 */
	void cShader::begin(uint* pass)		{	m_effect->begin(pass);		}
	void cShader::end()					{	m_effect->end();			}
	void cShader::beginPass(uint p)		{	m_effect->beginPass(p);		}
	void cShader::endPass()				{	m_effect->endPass();		}
	void cShader::lostDevice()			{	m_effect->lostDevice();		}
	void cShader::resetDevice()			{	m_effect->resetDevice();	}
	D3DXHANDLE cShader::insertTechnique(TCHAR const* name)
	{
		D3DXHANDLE handle = m_effect->getTechniqueByName(name);
		if (!handle)
			return NULL;
		m_handle.insert(name, handle);
		return handle;
	}
	D3DXHANDLE cShader::insertParameter(TCHAR const* name)
	{
		D3DXHANDLE handle = m_effect->getParameterByName(name);
		if (!handle)
			return NULL;
		m_handle.insert(name, handle);
		return handle;
	}
	void cShader::setTechnique(TCHAR const* name)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setTechnique(handle);
	}
	void cShader::setMatrix(TCHAR const* name, D3DXMATRIXA16 const* mat)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setMatrix(handle, mat);
	}
	void cShader::setMatrix(TCHAR const* name, sMatrix4 const* mat)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setMatrix(handle, mat);
	}
	void cShader::setMatrix(D3DXHANDLE handle, D3DXMATRIXA16 const* mat)
	{
		m_effect->setMatrix(handle, mat);
	}
	void cShader::setMatrix(D3DXHANDLE handle, sMatrix4 const* mat)
	{
		m_effect->setMatrix(handle, mat);
	}	
	void cShader::setMatrixArray(TCHAR const* name, D3DXMATRIXA16 const* mat, uint num)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setMatrixArray(handle, mat, num);
	}
	void cShader::setMatrixArray(TCHAR const* name, sMatrix4 const* mat, uint num)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setMatrixArray(handle, mat, num);
	}
	void cShader::setMatrixArray(D3DXHANDLE handle, D3DXMATRIXA16 const* mat, uint num)
	{
		m_effect->setMatrixArray(handle, mat, num);
	}
	void cShader::setMatrixArray(D3DXHANDLE handle, sMatrix4 const* mat, uint num)
	{
		m_effect->setMatrixArray(handle, mat, num);
	}
	void cShader::setVector(TCHAR const* name, D3DXVECTOR2 const* vec)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setVector(handle, vec);
	}
	void cShader::setVector(TCHAR const* name, D3DXVECTOR3 const* vec)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setVector(handle, vec);
	}
	void cShader::setVector(TCHAR const* name, D3DXVECTOR4 const* vec)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setVector(handle, vec);
	}
	void cShader::setVector(TCHAR const* name, sVector2 const* vec)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setVector(handle, vec);
	}
	void cShader::setVector(TCHAR const* name, sVector3 const* vec)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setVector(handle, vec);
	}
	void cShader::setVector(TCHAR const* name, sVector4 const* vec)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setVector(handle, vec);
	}
	void cShader::setVectorArray(TCHAR const* name, D3DXVECTOR4 const* vec, uint num)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setVectorArray(handle, vec, num);
	}
	void cShader::setFloat(TCHAR const* name, float f)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setFloat(handle, f);
	}
	void cShader::setFloatArray(TCHAR const* name, float const* f, uint num)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setFloatArray(handle, f, num);
	}
	void cShader::setInt(TCHAR const* name, int i)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setInt(handle, i);
	}
	void cShader::setIntArray(TCHAR const* name, int const* i, uint num)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setIntArray(handle, i, num);
	}
	void cShader::setBool(TCHAR const* name, BOOL b)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setBool(handle, b);
	}
	void cShader::setValue(TCHAR const* name, void* value, int size)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setValue(handle, value, size);
	}
	void cShader::setTexture(TCHAR const* name, cTexture const* tex)
	{
		D3DXHANDLE handle = NULL;
		m_handle.get(name, handle);
		setTexture(handle, tex);
	}
	void cShader::setTechnique(D3DXHANDLE handle)
	{
		m_effect->setTechnique(handle);
	}	
	void cShader::setVector(D3DXHANDLE handle, D3DXVECTOR2 const* vec)
	{
		if (checkIsNull(m_effect))
			return ;

		setValue(handle, (void*)vec, sizeof (D3DXVECTOR2));
	}
	void cShader::setVector(D3DXHANDLE handle, D3DXVECTOR3 const* vec)
	{
		if (checkIsNull(m_effect))
			return ;
		setValue(handle, (void*)vec, sizeof (D3DXVECTOR3));
	}
	void cShader::setVector(D3DXHANDLE handle, D3DXVECTOR4 const* vec)
	{
		m_effect->setVector(handle, vec);
	}
	void cShader::setVector(D3DXHANDLE handle, sVector2 const* vec)
	{
		if (checkIsNull(m_effect))
			return ;
		setValue(handle, (void*)vec, sizeof (sVector2));
	}
	void cShader::setVector(D3DXHANDLE handle, sVector3 const* vec)
	{
		if (checkIsNull(m_effect))
			return ;
		setValue(handle, (void*)vec, sizeof (sVector3));
	}
	void cShader::setVector(D3DXHANDLE handle, sVector4 const* vec)
	{
		m_effect->setVector(handle, vec);
	}
	void cShader::setVectorArray(D3DXHANDLE handle, D3DXVECTOR4 const* vec, uint num)
	{
		m_effect->setVectorArray(handle, vec, num);
	}
	void cShader::setFloat(D3DXHANDLE handle, float f)
	{
		m_effect->setFloat(handle, f);
	}
	void cShader::setFloatArray(D3DXHANDLE handle, float const* f, uint num)
	{
		m_effect->setFloatArray(handle, f, num);
	}
	void cShader::setInt(D3DXHANDLE handle, int i)
	{
		m_effect->setInt(handle, i);
	}
	void cShader::setIntArray(D3DXHANDLE handle, int const* i, uint num)
	{
		m_effect->setIntArray(handle, i, num);
	}
	void cShader::setBool(D3DXHANDLE handle, BOOL b)
	{
		m_effect->setBool(handle, b);
	}
	void cShader::setValue(D3DXHANDLE handle, void* value, int size)
	{
		m_effect->setValue(handle, value, size);
	}
	void cShader::setTexture(D3DXHANDLE handle, cTexture const* tex)
	{
		m_effect->setTexture(handle, tex);
	}
}



















